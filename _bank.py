# balance
balance = float("0")
while True:
    print("======BANK SYSTEM======")
    print("1. Show Balance")
    print("2. Deposit")
    print("3. Withdraw")
    print("3. exit")
    option = input("chose an option: ")

    # Show Balance
    if option == "1":
        print(f"Your Balance : {balance}")

    # Deposit
    elif option == "2":
        amount = float(input("Enter amount to add : $"))
        balance += amount
        print(f"{amount} added successfully!")

    # Withdraw
    elif option == "3":
        amount = float(input("Enter amount to Withdraw : $"))
        balance -= amount
        print(f"{amount} Withdraw successfully!")
        
    # exit
    elif option == "4":
        print("Exiting.....Godbye!")
        break
    # Invaild option
    else:
        print("Invaild option. Try Again.")

