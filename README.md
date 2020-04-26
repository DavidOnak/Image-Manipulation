# Image-Manipulation
An image can be altered using classes to represent a png and pixels, then editing these pixels. Additional effects can be added by representing a png as a binary tree of pixels and pruning it. 

# PNG as a Binary Tree
A PNG can be represented as a binary tree by cycling between verticle and horizontal splits until each leaf represents a single pixel. In this representation, the root would be the entire png and all children nodes would represent each half of their parent node.

# Prune image
Once a png represented as a binary tree, pruning the leaves based on a tolarance value given can created pixelated or paint brush effects. Each pixel or rectangle of a png has a score value which it determined from accumulated RGB colour values. If the children nodes from the same parent have scores within the given tolerance then the children will be pruned (removed) and the pixels from the parent node will become the children's average colour.

The 64 bit executable file in this repository called Prune Image can prune an image with about four megapixels as an upper bound size. To work it requires a directory where it is located called inputImages with the images to prune and writes pruned images into a directory called outputImages. The application will promt the user for the name of the image and tolerance to use.

The stats class caluates colour values of an image to allow for average colour and score calcuations. This class is used by the twoDtree class which creates the binary representation of a png. More details on prune and other methods the binary tree can do that are not used by the prune application can be read about in twoDtree.h.

![Original Image before prune](https://github.com/DavidOnak/Image-Manipulation/blob/master/inputImages/doggo.png?raw=true)

![After pruning with tolerance of 1000](https://github.com/DavidOnak/Image-Manipulation/blob/master/outputImages/pruned-doggo.png?raw=true)

# Manipulation with HSLA Pixels
Working on this right now, using HSLA pixels to represent a png effects can be done like grayscale or adding a watermark to an image
