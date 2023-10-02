# squareInCPP


ok uhh here's the thing that'll "help me" with the rotation



the distace between the posEx of 90 deg ->  x, y + (size + (size / 3)), x, y - (size + (size / 3)) 

and normal rot posEx -> (left) x - size, y - size, x - size, y + size
(right) x + size, y - size, x + size, y + size
(top) x - size, y - size, x + size, y - size
(bottom) x - size, y + size, x + size, y + size

or if right/top/bottom != NULL it will be set to the specified pos:
(b/t/l/r).x1, (b/t/l/r).y1, (b/t/l/r).x2, (b/t/l/r).y2



divided by 90 -> (normalRot - 90degRot) / 90

the quotient will be the amount you will multiply the rotation (that is less than 90 for now) 
-> if (rotation < 90){
   ((normalRot - 90degRot) / 90) * rotation
}

then the answer of this  ((normalRot - 90degRot) / 90) * rotation will be put into a newLine function to make the line(s) closer to the x,y coords set and add/subtract the answer of the equation to the lines size

i think

or maybe im just dumb

btw im too lazy to finish this today so ill do it tomorrow or the day after that idk lol
