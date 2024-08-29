/*
 Implement a web server that takes paths that look like

 /add?<somestring>

 and adds them to a running string joined by newlines, and returns the running string.
 
 So if you load these paths you get these corresponding responses:

 /add?hello

 hello


 /add?hi
 
 hello
 hi


 /add?words

 hello
 hi
 words

 You will have to keep track of a big char* array instead of a counter.
*/