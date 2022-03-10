import sys

def main():
    print("READ FROM STDIN OF CGI SCRIPT")
    for line in sys.stdin:
        print(line)

if __name__ == "__main__":
    main()