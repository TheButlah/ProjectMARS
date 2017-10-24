from __future__ import print_function
from __future__ import division
from __future__ import absolute_import

import tensorflow as tf
import numpy as np


def batch_norm(x, phase_train, decay=0.9, initial_beta=0.0, initial_gamma=1.0, scope='BN'):
    """Batch normalization layer.

    Used to stabilize distribution of outputs from a layer. Typically used right before a non-linearity. Works on
    n-dimensional data.

    Ref.: http://stackoverflow.com/questions/33949786/how-could-i-use-batch-normalization-in-tensorflow
    Ref.: https://arxiv.org/pdf/1502.03167.pdf

    Args:
        x:             Tensor,  shaped [batch, features...] allowing it do be used with Conv or FC layers of any shape.
        phase_train:   Boolean tensor, true indicates training phase.
        decay:         Float. The decay to use for the exponential moving average.
        initial_beta:  The initial target mean (offset). This will change as the model learns.
        initial_gamma: The initial target stddev (scale). This will change as the model learns.
        scope:         String or VariableScope to use as the variable scope.
    Returns:
        normed, vars:  `normed` is a tensor of the batch-normalized features, and has same shape as `x`.
                       `vars` is a dict of the variables.
    """
    with tf.variable_scope(scope):
        beta = tf.get_variable('Beta', initializer=tf.constant(initial_beta, shape=x.shape[-1]))  # Learned mean
        gamma = tf.get_variable('Gamma', initializer=tf.constant(initial_gamma, shape=x.shape[-1]))  # Learned std. dev.

        batch_mean, batch_var = tf.nn.moments(x, np.arange(len(x.shape)-1), name='Moments')
        ema = tf.train.ExponentialMovingAverage(decay=decay)

        def mean_var_with_update():
            ema_apply_op = ema.apply([batch_mean, batch_var])
            with tf.control_dependencies([ema_apply_op]):
                return tf.identity(batch_mean), tf.identity(batch_var)

        mean, var = tf.cond(
            phase_train,
            mean_var_with_update,
            lambda: (ema.average(batch_mean), ema.average(batch_var)))
        normed = tf.nn.batch_normalization(x, mean, var, beta, gamma, 1e-3, name='Normalized')
    return normed, {'Beta': beta, 'Gamma': gamma}


def dropout(x, phase_train, keep_prob=0.75, scope='Dropout'):
    """Dropout layer.

    Used to regularize Conv and FC layers by preventing co-adaptation of neurons. Works on n-dimensional data.
    Performs train-time scaling rather then inference-time scaling.

    Ref.: https://www.cs.toronto.edu/~hinton/absps/JMLRdropout.pdf

    Args:
        x:           Tensor, shaped [batch, features...] allowing it do be used with Conv or FC layers of any shape.
        phase_train: Boolean tensor, true indicates training phase. If false, no neurons are dropped.
        keep_prob:   Float, probability of dropping a neuron.
        scope:       String or VariableScope to use as the variable scope.
    Returns:
        A tensor with the neurons dropped.
    """
    with tf.variable_scope(scope):
        def train():
            # Figure out `shape` tensor for the dropout function
            dims = tf.unstack(tf.shape(x))
            dims[1:-1] = [1] * (len(dims) - 2)  # set all spatial dims to 1 so that they are all dropped together
            shape = tf.stack(dims)  # example: [batch, 1, 1, features]
            return tf.nn.dropout(x, keep_prob, shape, name='Train')

        def test():
            return tf.identity(x, name='Inference')

        return tf.identity(tf.cond(phase_train, train, test), name='Dropped')


def conv(x, num_features, size=3, activation=tf.nn.relu, phase_train=None, kernel_init=None, scope='Conv'):
    """"Convolutional Layer.

    Works on n spatial dimensions, as long as 1<=n<=3. Optionally performs batch normalization and also intelligently
    initializes the weights by using xavier initializer by default.

    Args:
        x:            Tensor, shaped [batch, spatial..., features]. Can have 1<=n<=3 spatial dimensions.
        num_features: The size of the feature dimension. This is the number of filters/neurons the layer will use.
        size:         The size of each convolutional filter.
        activation:   The activation function to use. If `None`, the raw scores are returned.
        phase_train:  If not `None`, then the scores will be put through a batch norm layer before getting fed into the
                      activation function. In that case, this will be a scalar boolean tensor indicating if the model
                      is currently being trained or if it is inference time.
        kernel_init:  The initializer to use for the weights of the kernel. If `None`, then Xavier initializer is used.
                      Ref.: http://proceedings.mlr.press/v9/glorot10a/glorot10a.pdf
        scope:        String or VariableScope to use as the variable scope.
    Returns:
        output, vars: `output` is a tensor of the output of the layer.
                      `vars` is a dict of the variables, including those in the batch norm layer if present.
    """
    with tf.variable_scope(scope):
        # Figure out `kernel_shape` tensor
        input_shape = tf.unstack(tf.shape(x))
        kernel_shape = [size]*(len(input_shape)-2)
        kernel_shape += [input_shape[-1], num_features]
        kernel_shape = tf.stack(kernel_shape)  # example: [size, size, input_features, num_features]

        # Set up initializer for `kernel`
        if kernel_init is None:
            # Use Xavier initialization.
            kernel_init = xavier_initializer

        kernel = tf.get_variable('Kernel', initializer=kernel_init(kernel_shape), validate_shape=False)
        kernel.set_shape(([size]*(len(input_shape)-2) + [None, num_features]))
        vars = {'Kernel': kernel}

        convolved = tf.nn.convolution(x, kernel, padding="SAME", name='Conv')

        # Do batch norm?
        if phase_train is not None:
            scores, bn_vars = batch_norm(convolved, phase_train)
            vars.update(bn_vars)
        else:
            scores = convolved

        # Do activation function?
        if activation is not None:
            result = activation(scores)
        else:
            result = scores

        return tf.identity(result, "Output"), vars


def xavier_initializer(shape, uniform=True, dtype=tf.float32, name='Xavier-Initializer'):
    """Outputs a random tensor initialized with the Xavier initializer. Ensures a variance of 1.

    This was already implemented in tf.contrib.layers, but it has been re-implemented here for simplification.
    Ref.: http://proceedings.mlr.press/v9/glorot10a/glorot10a.pdf

    Args:
        shape:  A tensor describing the shape of the output tensor.
        uniform: If a uniform or normal distribution should be used. Uniform performs slightly better empirically.
        dtype:   The type of tensor. Should be floating point.
        name:    The name of the initializer
    Returns:
        A tensor generated with the Xavier initializer.
    """
    shape = tf.convert_to_tensor(shape)  # Allow us to accept non-tensor inputs

    dims = shape.shape[0]
    if dims is 0:
        raise ValueError("`shape` cannot be a scalar!")
    elif dims is 1:
        # Need to clarify if this is correct, but it is how TensorFlow did it.
        n_avg = shape[0]
    else:
        # n_out is the number of neurons/filters/features
        n_out = shape[-1]
        # n_in is the number of weights associated with a neuron. The following code works both for Conv and FC layers.
        n_in = tf.reduce_prod(shape[:-1])
        n_avg = tf.cast(n_in + n_out, dtype) / 2.0

    if uniform:
        limit = tf.sqrt(3.0 / n_avg)
        return tf.random_uniform(shape, -limit, limit, dtype=dtype, name=name)
    else:
        stddev = tf.sqrt(1.3 / n_avg)  # This corrects for the fact that we are using a truncated normal distribution.
        return tf.truncated_normal(shape, stddev=stddev, dtype=dtype, name=name)
