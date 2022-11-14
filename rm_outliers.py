import csv
import numpy as np


def get_average(lst: list[int]):
    print(lst)
    return sum(lst) / len(lst)


def load_data(dir_name: str, file_name: str):
    file_path = dir_name + "/" + file_name
    time_list = []
    prd_list = []

    vertexCount: int = -1

    with open(file_path, "r", newline="") as csvfile:
        file_reader = csv.reader(
            csvfile, delimiter=",", quotechar="|", quoting=csv.QUOTE_MINIMAL
        )

        # skip header
        next(file_reader)

        for lines in file_reader:
            vertexCount = int(lines[1])
            time_list.append(int(lines[2]))
            prd_list.append(float(lines[5]))

    print(prd_list)

    return time_list, prd_list, vertexCount


def rm_outliers(data: list[int]) -> list[int]:
    """
    removes outliers from the list
    """

    k = 1.5

    q1 = np.percentile(data, 25)  # Q1
    q3 = np.percentile(data, 75)  # Q3
    iner_quartile = q3 - q1
    lower_bound = q1 - k * iner_quartile
    upper_bound = q3 + k * iner_quartile

    data_rm = filter(lambda x: x > lower_bound and x < upper_bound, data)
    return list(data_rm)


def save_results(results, dir_name, file_name):
    file_path = dir_name + "/" + file_name

    with open(file_path, "w", newline="") as csvfile:
        file_writer = csv.writer(
            csvfile, delimiter=",", quotechar="|", quoting=csv.QUOTE_MINIMAL
        )
        file_writer.writerow(
            [
                "nazwa instancji",
                "liczba wierzchołków",
                "średni czas [ns], średni błąd [%]",
            ]
        )
        file_writer.writerows(results)


def main():
    dir_name = "./results"

    result_files = [
        "m6.atsp.csv",
        "m10.atsp.csv",
        "burma14.tsp.csv",
        "gr24.tsp.csv",
        "bays29.tsp.csv",
        "ftv33.atsp.csv",
        "ftv44.atsp.csv",
        "att48.tsp.csv",
        "berlin52.tsp.csv",
        "ftv70.atsp.csv",
        "gr96.tsp.csv",
        "kroA100.tsp.csv",
        "gr120.tsp.csv",
        "pr124.tsp.csv",
        "kroB150.tsp.csv",
        "pr152.tsp.csv",
        "ftv170.atsp.csv",
        "kroB200.tsp.csv",
        "rbg323.atsp.csv",
    ]

    results = []
    for file_name in result_files:

        print(file_name)

        time_list, prd_list, vertexCount = load_data(dir_name, file_name)
        time_list = rm_outliers(time_list)

        average_time = get_average(time_list)
        average_prd = get_average(prd_list)

        average_time = round(average_time, 2)
        average_prd = round(average_prd, 2)

        results.append([file_name, vertexCount, average_time, average_prd])

    save_results(results, "./", "results_average.csv")


if __name__ == "__main__":
    main()
