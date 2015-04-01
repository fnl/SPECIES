SPECIES
=======

A standalone command line application capable of identifying taxonomic mentions in documents and mapping them to corresponding NCBI Taxonomy database entries.

Pafilis E, Frankild SP, Fanini L, Faulwetter S, Pavloudi C, et al. **The SPECIES and ORGANISMS Resources for Fast and Accurate Identification of Taxonomic Names in Text.** (2013) [PLoS ONE 8(6): e65390](http://www.plosone.org/article/info:doi%2F10.1371%2Fjournal.pone.0065390)

This is a GitHub mirror of the SPECIES tagger with *modifications* of the tool. To get the original, unmodified code, please download it from its [homepage](http://species.jensenlab.org/).

The modified program parses a simpler input format (one citation per line, with PMID-title-abstract, tab-separated) and outputs the detected TaxIDs attached as an extra (fourth) field of comma-separated Taxonomy IDs for each input citation.
In addition, the added script `replace_tags.py` makes it possible to replace that list of TaxIDs in the output with the labels `TRUE`, `FALSE`, or `NULL`.
