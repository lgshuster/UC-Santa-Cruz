# Leonid Shuster
# Jongha Choi

import time

hangman = (
"""
-----
|   |
|
|
|
|
|
|
|
--------
""",
"""
-----
|   |
|   0
|
|
|
|
|
|
--------
""",
"""
-----
|   |
|   0
|  -+-
|
|
|
|
|
--------
""",
"""
-----
|   |
|   0
| |-+-
|
|
|
|
|
--------
""",
"""
-----
|   |
|   0
| |-+-|
|
|
|
|
|
--------
""",
"""
-----
|   |
|   0
| |-+-|
|   |
|
|
|
|
--------
""",
"""
-----
|   |
|   0
| |-+-|
|   |
|   |
|
|
|
--------
""",
"""
-----
|   |
|   0
| |-+-|
|   |
|   |
|  |
|
|
--------
""",
"""
-----
|   |
|   0
| |-+-|
|   |
|   |
|  |
|  |
|
--------
""",
"""
-----
|   |
|   0
| |-+-|
|   |
|   |
|  | |
|  |
|
--------
""",
"""
-----
|   |
|   0
| |-+-|
|   |
|   |
|  | |
|  | |
|
--------
""")

#replaces dash with letter depending on how many times it shows up in the word
def replace_dash_with_letter():
    if word.count(guess) > 1:                   #checking if letter repeats more than once
        for letter in range(len(word)):
            if guess == word[letter]:                   #getting the position of the letter in the word
                letter_list[letter] = guess
                dash_list = " ".join(map(str, letter_list))
        print(dash_list)

    else:                   #if letter shows up only once in the word
        position = word.index(guess)
        letter_list[position] = guess
        dash_list = " ".join(map(str, letter_list))
        print(dash_list)

#prints updated list of guesses
def update_guesses():                   #checking if user has any previous guesses
    time.sleep(1)
    updated_list = ", ".join(map(str, guesses_list))
    print("Letters used so far are:", updated_list)
    print("")

print("Welcome to Hangman!")
print("")
time.sleep(0.5)

word = input("Player 1, what is your word? [you can put multiple words] ").lower()

for line in range(12):                  #hiding the word from the second player
    print()

letter_list = []

print("")

for letter in range(len(word)):                     #creating a list of dashes
    if word[letter] == " ":                     #if input is more than one word
        letter_list.append(" ")
    else:
        letter_list.append("_")

dash_list = " ".join(map(str, letter_list))
print(dash_list)

guesses_list = []
wrong_tries = 0

while True:                     #games keep playing until player wins or loses
    print("")

    if len(guesses_list) > 0:
        update_guesses()

    guess = input("Player 2, what letter would you like to guess? (press 1 if you want to guess the word) ").lower()
    print("")

    if guess == "1":
        new_guess = input("What is your guess? ").lower()
        time.sleep(1)

        if new_guess == word:                   #if player guesses word correctly
            print("")
            print("Correct! Player 2 has won the game")
            break

        else:                   #if player doesn't guess word correctly
            print("Incorrect guess")
            wrong_tries += 1
            print(hangman[wrong_tries])

            if wrong_tries == 10:                   #if player has 10 wrong tries
                print("You have died, Player 1 is dead")
                break

    else:
        if len(guess) > 1:                  #checking if guess is more than one letter
            print("Too many letters! ")

        else:
            if guess not in guesses_list:                   #checking if player already guessed letter
                time.sleep(1)

                if guess in word:                   #checking if letter is in the word
                    print("Correct!")
                    print("")
                    replace_dash_with_letter()

                else:                   #if guess is incorrect
                    print("Incorrect guess")
                    wrong_tries += 1
                    print(hangman[wrong_tries])

                    if wrong_tries == 10:                   #if player has 10 wrong tries
                        print("You have died, Player 1 is dead")
                        break

                guesses_list.append(guess)

            else:                   #if player already guessed the letter
                print("You've already guessed this letter!")

            if "_" not in letter_list:                  #if player guesses all the letters
                print("Congrats! Player 2 has won the game")
                break
