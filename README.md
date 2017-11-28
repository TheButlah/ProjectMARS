# Layers
Layers is a library for Neural Networks in TensorFlow. It provides implementations for a variety of neural network layers and other helper functions in TensorFlow. It is designed with an emphasis on being well documented, cleanly written, and as generalizable as possible to new network architectures. To this end, many tricks and techniques are used to ensure the best-practice methods of defining these layers are used by default.

# List of functions implemented
- Fully Connected Layer
  - Generalizes to N>=0 spatial dimensions
  - Uses Xavier initialization by default
  - Uses batch normalization by default
- Batch Normalization Layer
  - Generalizes to N>=0 spatial dimensions
- Dropout Layer
  - Generalizes to N>=0 spatial dimensions
- Convolutional Layer
  - Generalized to 1<=N<=3 spatial dimensions (due to limitations in `tf.convolution`)
  - Initializes using Xavier initialization by default
  - Uses batch normalization by default
- Pooling Layer
  - Generalizes to N>=1 spatial dimensions
  - Can compute a mask for the selected indices when max pooling
- Unpooling Layer
  - Performs the unpooling technique described in the [SegNet Paper](https://arxiv.org/abs/1511.00561)
  - Provides a novel generalization to N>=1 spatial dimensions
- Xavier initializer
  - Provides a tensor to be used as the intializer for the network weights as described in [this paper](http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.207.2059&rep=rep1&type=pdf).
- K-Competitive Layer
  - Implements the k-competitive layer for fully connected networks described in the [KATE paper](https://arxiv.org/abs/1705.02033).
  - Provides a novel generalization for it for N>=0 spatial dimensions.
