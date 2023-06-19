import requests
import sys
import threading
import time

# Get program arguments
args = sys.argv

# Usage: python3 tester.py [host] [nbOfRequests] [parallel]

if (len(args) != 4):
    host = input("Enter host: ")

    # Check if the host can be reached
    try:
        r = requests.get(host)
    except:
        print("Host unreachable")
        exit()

    nbOfRequests = int(input("Nb of requests: "))
    parallel = int(input("Parallel: "))
else:
    host = args[1]
    try:
        r = requests.get(host)
    except:
        print("Host unreachable " + host)
        exit()
    nbOfRequests = int(args[2])
    parallel = int(args[3])

# Print the host
print("Host: " + host)

# Parallel requests
threads = []

# Timing statistics
times = []
minTime = 1e60
avgTime = 0
maxTime = 0

def request(host, number = -1):
    start = time.time()
    r = requests.get(host)
    end = time.time()

    # Update timing statistics
    ms = int((end - start) * 1000)
    times.append(ms)
    global minTime
    minTime = min(minTime, ms)
    global maxTime
    maxTime = max(maxTime, ms)

    print(f"{number} GET {host} {r.status_code} {ms}ms")

def thread_function():
    for i in range(nbOfRequests // parallel):
        request(host, i)

# Launch all threads at the same time
for i in range(parallel):
    x = threading.Thread(target=thread_function)
    threads.append(x)
    x.start()

for i in range(parallel):
    threads[i].join()

# Compute average time
avgTime = sum(times) / len(times)

# Print timing statistics
print(f"Min time: {minTime}ms")
print(f"Avg time: {avgTime}ms")
print(f"Max time: {maxTime}ms")
