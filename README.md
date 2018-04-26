# LibffmGenerator

Script for converting tabular csv data into the data format needed for Field-Aware Factorization Machines. Libraries supporting Field-Aware Factorization Machines are provided with libraries like <a href="https://github.com/guestwalk/libffm">LibFFM</a> and <a href="http://xlearn-doc.readthedocs.io/en/latest/">xLearn</a>.

This script supports up to 3 files (train / validation / test). The label is expected to be in the first column and numerical columns in the following columns. Categorical columns are expected to be last. The number of numeric columns must be specified with the parameter `--numeric`.

The first file given is expected to be the training set. Validation and test sets can be specified by the use of the arguments `--valid`and `--test`. To speed up memory allocation the number of samples per set can be specified. This is possible by the use of the parameters `--n_train`, `--n_valid` and `--n_test`.

An example would look like: <br>
`LibffmGenerator.out train.csv --valid valid.csv --test test.csv --n_train 70000 --n_valid 10000 --n_test 20000`
