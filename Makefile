CFLAGS=-g -I. 

mysh: mysh.o
	$(CC) -o  $@ $^ 
clean:
	$(RM) *.o
mostlyclean: clean
	$(RM) *~

