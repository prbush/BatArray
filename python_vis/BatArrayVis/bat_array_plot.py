# This is a sample Python script.

# Press ⌃R to execute it or replace it with your code.
# Press Double ⇧ to search everywhere for classes, files, tool windows, actions, and settings.

import matplotlib.pyplot as plot
import sys
import numpy as np


def plotTheThings(filepath, numSamples, offset, plotAllChannels):
    channels = [[], [], [], [], [], [], [], []]
    time = []

    with open(filepath, "rb") as rawFile:

        rawFile.seek(offset, 0)
        for i in range(numSamples):
            time.append(i)
            for j in range(8):
                bytes = rawFile.read(2)
                sample_int = int.from_bytes(bytes = bytes, byteorder = "little", signed = True)
                sample_float = sample_int * (5 / 32768)

                channels[j].append(sample_float)

    if not plotAllChannels:
        # plot channel 1
        plot.stem(time, channels[0], label="Channel 1")
        ax = plot.gca()
        ax.set_ylim(-7.5, 7.5)
        plot.show()
    else:
        # plot all channels
        fig, (ch1, ch2, ch3, ch4, ch5, ch6, ch7, ch8) = plot.subplots(8)
        fig.suptitle('Channels 1-8')
        ch1.stem(time, channels[0])
        ch2.stem(time, channels[1])
        ch3.stem(time, channels[2])
        ch4.stem(time, channels[3])
        ch5.stem(time, channels[4])
        ch6.stem(time, channels[5])
        ch7.stem(time, channels[6])
        ch8.stem(time, channels[7])

        ch1.set_ylim(-7.5, 7.5)
        ch2.set_ylim(-7.5, 7.5)
        ch3.set_ylim(-7.5, 7.5)
        ch4.set_ylim(-7.5, 7.5)
        ch5.set_ylim(-7.5, 7.5)
        ch6.set_ylim(-7.5, 7.5)
        ch7.set_ylim(-7.5, 7.5)
        ch8.set_ylim(-7.5, 7.5)

        plot.show()


def main(params):
    filepath = params[0]
    numSamples = int(params[1])
    offset = int(params[2])
    allChannels = params[3].lower() == 'true'

    plotTheThings(filepath, numSamples, offset, allChannels)


# Press the green button in the gutter to run the script.
if __name__ == '__main__':
    main(sys.argv[1:])

# See PyCharm help at https://www.jetbrains.com/help/pycharm/
