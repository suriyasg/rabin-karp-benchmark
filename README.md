## Running the code

```
cd build/
cmake ..
make
```
To run benchmark with data
```
./benchmark_runner --text ../data/generated_sample.txt --csv ../data/long_1000_char_dataset_from_generated.csv --offset 50 --count 1
```
This gets data from `../data/generated_sample.txt` and runs 50th sample in the `../data/long_1000_char_dataset_from_generated.csv`