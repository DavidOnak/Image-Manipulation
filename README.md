# Image-Manipulation
Using classes to represent images and pixels, images can be manipulated by editing their pixels. Also by representing an image as a tree of pixels, pruning can be done to create cool effects on images. 

# Prune image
The stats and twoDtree classes allow for an image to be pruned. The stats class stores information on accumulated values for each colour and can return average values and scores for a given rectangle on an image. Meanwhile the twoDtree class creates a binary tree that represents an image.

Performing a prune can be done by the twoDtree class, which removes leaves from the tree. This is done with a tolerance that is passed through and will remove leaves that have colour value differneces within tolerance with its parent node. These values are determined from using the stats class.

The result of a prune increases pixilation of the outputed image by an amount depending on what the set tolerance is. This can add a cool artistic look to an image.
