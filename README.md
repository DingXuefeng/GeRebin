# GeRebin
rebinning program for HPGe aquired data.

The input are txt files. they loaded into the memory by InputManager. In order to easily load data of different format, an interface is provided. I provide the implemntation for loading GeMPI data.

An interface for rebinning handler is provided to allow different strategies. The program provide four strategies: output the energies with equal width binning or non-equal width binning; output the ADC with equal width binning or non-equal width binning.
