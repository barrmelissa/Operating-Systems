#imports
import sys
import os
import string
import random

#gives us only the lowercase letters of the alphabet
string.ascii_lowercase

#opens 3 files with writing permissions, if they do not exist, creates them
test_file1 = open("file1.txt", "w+")
test_file2 = open("file2.txt", "w+")
test_file3 = open("file3.txt", "w+")

#gets 10 letters that are randomly chosen
for x in range(10):
    random_letter = str(random.choice(string.ascii_lowercase))
    #writes the letters into the file
    test_file1.write(random_letter)
    #writes the letters to stdout
    sys.stdout.write(random_letter)
    sys.stdout.flush()

#adds new line characters to the end of the file and std out
test_file1.write("\n")
sys.stdout.write('\n')
sys.stdout.flush()

#gets 10 letters that are randomly chosen
for x in range(10):
    random_letter = str(random.choice(string.ascii_lowercase))
    #writes the letters into the file
    test_file2.write(random_letter)
    #writes the letters to stdout
    sys.stdout.write(random_letter)
    sys.stdout.flush()

#adds new line characters to the end of the file and std out
test_file2.write("\n")
sys.stdout.write('\n')
sys.stdout.flush()

#gets 10 letters that are randomly chosen
for x in range(10):
    random_letter = str(random.choice(string.ascii_lowercase))
    #writes the letters into the file
    test_file3.write(random_letter)
    #writes the letters to stdout
    sys.stdout.write(random_letter)
    sys.stdout.flush()

#adds new line characters to the end of the file and std out
test_file3.write("\n")
sys.stdout.write('\n')
sys.stdout.flush()

#closes all 3 files
test_file1.close()
test_file2.close()
test_file3.close()

#gets 2 random numbers between 1 and 42 inclusive
random_num1 = random.randint(1,43)
random_num2 = random.randint(1,43)
#prints random numbers to stdout
print(random_num1)
print(random_num2)
#gets the product of the two random numbers
product = random_num1 * random_num2
print(product)
