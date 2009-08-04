'''
    Date: 20th July 2009
    This test ensures that the engine is able to run and shut down properly.
    
    The criteria is that you should be able to "exit" the mainloop twice,
    either by manual exit (ESC) or the console command "exit".
    Before each run a welcoming message will be printed.
'''
import traceback
import smug
import time

def main():

    try:
        smug.initialize()
    except:
        print "An error occured while initializing smug. Exiting."
        traceback.print_exc()
        return

    print "Hello World!"
    
    try:
        smug.run()
    except:
        print "An error occured while running smug. Exiting."
        traceback.print_exc()
        return
            
    
    try:
        smug.terminate()
    except:
        print "An error occured while terminating smug. Exiting."
        traceback.print_exc()
        return        

    
