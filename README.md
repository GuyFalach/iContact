# iContact
An image processing project that allows the user to control the mouse's movement with their eyes. The project was made using ```C++``` and ```winAPI```. In the project I programed all the image processing functions on my own without the help of outsize libraries such as ```openCV```. Moreover, the project was not done using machine learning, making it harder and more challenging. I implemented all the mathematical equations and algorithms.

# How to use
Make sure you have ```C++14``` installed on your computer and run source.cpp
Place your eyes in the center of your screen and move them according to the direction of the marked circles until beep sounds are made. Afterwards, move you facce according to the direction that you want the mouse to move, and stop its action by winking. In order to click, you should wink once. Left winking means left click and right winking means right click.
If three beeps sounds are made, it means that the program failed to loacate the eyes of the user. Thus it decided to reapeat the recalibration process. 
In order to terminate the program you should turn off your online camera.

# How the project works
The the project uses the operators: ```Sobel operator```, ```Canny edge detector``` in order to find all the boarders. 
Movement deteaction is being used constantly in oreder to save computer resources while the user does not wish for any action to be done. 

## Architectural structure
![hardware drawio](https://user-images.githubusercontent.com/106104471/169879736-aa10b9eb-58a0-448a-9660-64fe1cf875db.png)
