import csv
import sys

# Count a single STR tha a sequence contains
def count_one_str(STR, sequence):
    count_max = 0
    count = 0
    begin = 0
    while True:
        index = sequence.find(STR, begin)
        if index == -1:
            return count_max

        count = count + 1 if index == begin else 1
        begin = index + len(STR)
        count_max = count if count > count_max else count_max
    return 0

# Count all STRs that a sequence contains
def count_all_strs(strs, sequence):
    counts = dict(zip(strs, [0] * len(strs)))
    for STR in strs:
        counts[STR] = count_one_str(STR, sequence)

    return counts

def main():
    # Receive command line arguments
    if (len(sys.argv) != 3):
        print("Usage: python dna.py data.csv sequence.txt")
        sys.exit(1)

    # Read all STRs
    strs = []
    with open(sys.argv[1]) as f:
        reader = csv.reader(f)
        strs = next(reader)
    del strs[0]

    # Read DNA sequence
    with open(sys.argv[2]) as f:
       sequence = f.read()

    # Count STRs
    counts = count_all_strs(strs, sequence)

    # Compare STR counts with each person in database
    with open(sys.argv[1]) as f:
        reader = csv.DictReader(f)

        for line in reader:
            flag = True
            for STR in strs:
                if int(line[STR]) != counts[STR]:
                    flag = False
                    break
            if flag:
                print(line["name"])
                break

        if not flag:
            print("No match")

main()

