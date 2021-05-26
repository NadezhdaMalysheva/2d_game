from skimage.io import imread, imsave
import numpy as np

def res (img, path):
     #img = imread(path)
     img1 = np.ones((int(img.shape[0]*2), int(img.shape[1]*2),4))
     img1[::2,::2,] = img
     img1[1::2,::2,] = img
     img1[::2,1::2,] = img
     img1[1::2,1::2,] = img
     imsave(path, img1.astype('uint8'))
