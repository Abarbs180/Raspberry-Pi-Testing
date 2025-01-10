import time
from datetime import datetime

def main():
    print("Starting Raspberry Pi Logging Test...")
    try:
        while True:
            current_time = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
            print(f"[{current_time}] Raspberry Pi is running!")
            time.sleep(1)  # Wait for 1 second
    except KeyboardInterrupt:
        print("\nLogging stopped. Goodbye!")

if __name__ == "__main__":
    main()

