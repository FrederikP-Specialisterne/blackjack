# Calculator functionalities

def add(n1,n2):
    return n1 + n2

def subtract(n1,n2):
    return n1 - n2

def multiply(n1,n2):
    return n1 * n2

def divide(n1,n2):
    return n1 / n2




if __name__ == "__main__":
    while True: # Main loop
        while True: # Keep asking for inputs until valid input has been entered
            try:
                inp = input("Enter a number, and operation, and a second number (e.g. \"2 plus 2\")\n")
                ops = inp.split(" ")
            except:
                print("Something went horribly, horribly wrong... Try again")
                continue
            
            num_ops = len(ops)
            if num_ops != 3:
                print(f"The input should have 3 parts. The given input (\"{inp}\") has {num_ops}")
                continue


            break

        if inp == "q":
            quit
        else:
            print("Program succesfully ran to the end")