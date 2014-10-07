yaflengine
==========

Yet Another Fuzzy Logic engine (Yaflengine).


This library contains a fuzzy logic framework for performing calculations. Models can bu stored in plain text files for future access, or can be hard-coded. It allows model nesting, where the output of some model becomes the input of another. The rule system is pretty easy to grasp, with sentences like:

if temperature.high then fan.full_power

It also allows for modifiers such "very" and "little". An example:

if temperature.very.high then fan.full_power

This package uses another library I created for loading and parsing Windows ".ini" style files.
