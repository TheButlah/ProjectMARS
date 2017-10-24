import tensorflow as tf
import numpy as np


def batch_norm(x, phase_train, decay=0.9, scope='BN'):
    """Batch normalization layer, Used to stabilize distribution of outputs from a layer.
    Typically used right before a non-linearity. Works on n-dimensional data.
    Ref.: http://stackoverflow.com/questions/33949786/how-could-i-use-batch-normalization-in-tensorflow
    Note: The original author's code has been modified to generalize the spatial dimensions of the input tensor.

    Args:
        x:            Tensor,  with batch as first dimension (e.g. BC or BXYZC). Can be from FC or Conv Layers.
        phase_train:  Boolean tensor, true indicates training phase
        decay:        Float. The decay to use for the exponential moving average.
        scope:        String or VariableScope to use as the variable scope
    Returns:
        normed, vars: `normed` is the batch-normalized features, and has same shape as `x`.
                      `vars` is a dict of the variables.
    """
    with tf.variable_scope(scope):
        beta = tf.get_variable('beta', initializer=tf.constant(0.0, shape=x.shape[-1]))  # Learned mean
        gamma = tf.get_variable('gamma', initializer=tf.constant(1.0, shape=x.shape[-1]))  # Learned variance

        batch_mean, batch_var = tf.nn.moments(x, np.arange(len(x.shape)-1), name='Moments')
        ema = tf.train.ExponentialMovingAverage(decay=decay)

        def mean_var_with_update():
            ema_apply_op = ema.apply([batch_mean, batch_var])
            with tf.control_dependencies([ema_apply_op]):
                return tf.identity(batch_mean), tf.identity(batch_var)

        mean, var = tf.cond(phase_train,
                            mean_var_with_update,
                            lambda: (ema.average(batch_mean), ema.average(batch_var)))
        normed = tf.nn.batch_normalization(x, mean, var, beta, gamma, 1e-3)
    return normed, {'beta': beta, 'gamma': gamma}


