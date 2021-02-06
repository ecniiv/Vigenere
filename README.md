# Simple Vigenere crack

Based on the Kasiski examination, the program consists of looking for many repetitions in the ciphertext, then perform substitution with the most frequent letters.

# Usage


```
./main <file> [--key | --kfile] [--min-key-length] [--max-key-length]
```

# Examples

```
./main ciphertexts/orthographe.enc --min-key-length 4 --max-key-length 11
```

```
./main ciphertexts/debian --key DEBIANFTW
```
