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
        file_reader = csv.reader(csvfile, delimiter=",", quoting=csv.QUOTE_MINIMAL)

        # skip header
        next(file_reader)

        for lines in file_reader:
            vertexCount = int(lines[1])
            time_list.append(int(lines[2]))
            prd_list.append(float(lines[3]))

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
        file_writer = csv.writer(csvfile, delimiter=",", quoting=csv.QUOTE_MINIMAL)
        file_writer.writerow(
            [
                "nazwa instancji",
                "liczba wierzchołków",
                "średni czas [ns]",
                "średni błąd [%]",
            ]
        )
        file_writer.writerows(results)


def main():
    dir_name = "./results"

    instance_names = [
        # Error = 0.0
        "burma14.tsp",
        "gr17.tsp",
        "gr21.tsp",
        "gr24.tsp",
        "bays29.tsp",
        "ftv33.atsp",
        "ftv44.atsp",
        "ft53.atsp",
        "ftv70.atsp",
        # Error <= 0.5
        "ch150.tsp",
        "ftv170.atsp",
        "gr202.tsp",
        "rbg323.atsp",
        # Error <= 1.5
        "pcb442.tsp",
        "rbg443.atsp",
        "gr666.tsp",
        "pr1002.tsp",
        "pr2392.tsp",
    ]

    results = []
    for instance_name in instance_names:

        file_name = f"{instance_name}.csv"
        time_list, prd_list, vertexCount = load_data(dir_name, file_name)

        average_time = get_average(time_list)
        average_prd = get_average(prd_list)

        average_time = round(average_time, 2)
        average_prd = round(average_prd, 2)

        row = [instance_name, vertexCount, average_time, average_prd]
        results.append(row)

    print(results)
    save_results(results, "./", "results_average.csv")


if __name__ == "__main__":
    main()
