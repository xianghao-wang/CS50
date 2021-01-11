def balanceable(numbers, index, sum_w):
    if index == len(numbers):
        return sum_w == 0

    return balanceable(numbers, index+1, sum_w-numbers[index]) or balanceable(numbers, index+1, sum_w+numbers[index])

def balanceable(numbers):
    sum_w = 0
    for number in numbers:
        sum_w += number

    if sum_w % 2 == 1:
        return False

    l = 0, r = 0
    for number in reversed(sorted(numbers)):
        sum_w -= number
        if l + number == r + sum_w:
            return True
        elif l + number > r + sum_w:
            r += number
        elif l + number < r + sum_w:
            l += number

numbers = [5]
print(balanceable(numbers, 0, 0))