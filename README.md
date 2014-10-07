yaflengine
==========

Yet Another Fuzzy Logic engine (Yaflengine) for C++.


This library contains a fuzzy logic framework for performing calculations. Models can be stored/created in plain text files or can be hard-coded. It allows model nesting, where the output of some model becomes the input of another. The rule system is pretty easy to grasp, with sentences like:

if temperature.high then fan.full_power

It also allows for modifiers such "very" and "little". An example:

if altitude.very.high and speed.low then pitch.down

This package uses another library I created for loading and parsing Windows ".ini" style files.
