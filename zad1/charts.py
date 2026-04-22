import os
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker

RESULTS_DIR = "results"
DATASETS = ["nonprintable", "regulartests", "repeatingword", "spaces"]
SIZES = ["16K", "64K", "256K", "1M", "4M"]
PROGRAMS = ["normalize1", "normalize2", "normalize3", "normalize4", "normalize5", "normalize6"]

SIZE_BYTES = {"16K": 16*1024, "64K": 64*1024, "256K": 256*1024, "1M": 1024*1024, "4M": 4*1024*1024}
MEMORY_USAGE = {
    "normalize1": 10248,
    "normalize2": 8728,
    "normalize3": 6808,
    "normalize4": 6812,
    "normalize5": 3520,
    "normalize6": 5600
}
def load_results():
    data = {}
    for dataset in DATASETS:
        data[dataset] = {prog: [] for prog in PROGRAMS}
        for size in SIZES:
            path = os.path.join(RESULTS_DIR, f"{dataset}_{size}.txt")
            if not os.path.exists(path):
                for prog in PROGRAMS:
                    data[dataset][prog].append(None)
                continue
            times = {}
            with open(path) as f:
                for line in f:
                    line = line.strip()
                    if line.startswith("size="):
                        continue
                    parts = line.split()
                    if len(parts) == 2:
                        times[parts[0]] = float(parts[1])
            for prog in PROGRAMS:
                data[dataset][prog].append(times.get(prog))
    return data

def make_charts(data):
    x = [SIZE_BYTES[s] for s in SIZES]
    x_labels = SIZES
    colors = plt.cm.tab10.colors

    for dataset in DATASETS:
        fig, ax = plt.subplots(figsize=(9, 6))
        fig.suptitle(f"Normalize — czas wykonania [{dataset}]", fontsize=13)

        for i, prog in enumerate(PROGRAMS):
            y = data[dataset][prog]
            valid = [(xi, yi) for xi, yi in zip(x, y) if yi is not None]
            if not valid:
                continue
            xs, ys = zip(*valid)
            ax.plot(xs, ys, marker='o', label=prog, color=colors[i])

        ax.set_xlabel("Rozmiar danych")
        ax.set_ylabel("Czas [s]")
        ax.set_xscale("log", base=2)
        ax.set_xticks(x)
        ax.set_xticklabels(x_labels)
        ax.xaxis.set_major_formatter(ticker.FixedFormatter(x_labels))
        ax.legend()
        ax.grid(True, which="both", linestyle="--", alpha=0.5)

        plt.tight_layout()
        fname = f"chart_{dataset}.png"
        plt.savefig(fname, dpi=150)
        print(f"Saved {fname}")
        plt.close()

def make_memory_chart():
    colors = plt.cm.tab10.colors
    fig, ax = plt.subplots(figsize=(8, 5))
    fig.suptitle("Normalize — szczytowe zużycie pamięci (regulartests/test1M)", fontsize=13)

    progs = list(MEMORY_USAGE.keys())
    values = [MEMORY_USAGE[p] / 1024 for p in progs]  # KB -> MB

    bars = ax.bar(progs, values, color=colors[:len(progs)])
    ax.set_xlabel("Wersja")
    ax.set_ylabel("Pamięć [MB]")
    ax.bar_label(bars, fmt="%.2f MB", padding=3)
    ax.grid(True, axis="y", linestyle="--", alpha=0.5)

    plt.tight_layout()
    plt.savefig("chart_memory.png", dpi=150)
    print("Saved chart_memory.png")
    plt.close()

if __name__ == "__main__":
    data = load_results()
    make_charts(data)
    make_memory_chart()
