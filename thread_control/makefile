.PHONY:grab
grab:grab.o
	gcc grab.o -o grab -lpthread
grab.o:Ticket_Grabbing.c
	gcc -c Ticket_Grabbing.c -o grab.o

clean:
	rm -f grab grab.o


