# Held-Karp algorithm solving the Travelling Salesman Problem

## Measure RAM usage over time:

```bash
valgrind --tool=massif ./bin/main
```

## Get max RAM usage

Read "Maximum resident set size (kbytes)" value from:

```bash
/usr/bin/time -v ./bin/main
```
