'''
    Date: 20th July 2009
    This test ensures that the engine is able to run and shut down properly.
    
    The criteria is that you should be able to "exit" the mainloop twice,
    either by manual exit (ESC) or the console command "exit".
    Before each run a welcoming message will be printed.
'''

import smug
import time

def main():

    smug.initialize()

    print "Hello World!"

    smug.run()

    print "Ahoy-hoy!"

    smug.run()
    
    smug.terminate()
    
