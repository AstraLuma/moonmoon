NUM_PHASES = 28
NUM_LEDS = 5
MAX_LED = 0xFFFF

# We're just going to assume that the progression is linear.


def breakup(val):
    """
    Takes a value 0-1 and produces a sequence of LED values.
    """
    if val < 0.5:
        amount_lit = val * 2
        waning = False
    else:
        amount_lit = (1 - val) * 2
        waning = True

    increment = 1 / NUM_LEDS
    leds = []
    while amount_lit > increment:
        leds.append(1.0)
        amount_lit -= increment
    leds.append(amount_lit)

    while len(leds) < NUM_LEDS:
        leds.append(0)

    leds = leds[:NUM_LEDS]  # It can add an extra, cut it off it's ~0

    if waning:
        return reversed(leds)
    else:
        return leds


print("{")

for phase in range(NUM_PHASES):
    print("    {" + ', '.join(f"0x{int(n * MAX_LED):04X}" for n in breakup(phase / NUM_PHASES)) + "},")

print("}")
