import sys
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from pathlib import Path

# Accepte le chemin CSV en argument, sinon utilise le fichier par défaut
csv_path = Path(sys.argv[1]) if len(sys.argv) > 1 else Path(__file__).parent / "simuLJ.csv"
df = pd.read_csv(csv_path)

# Détection automatique des noms de particules depuis les colonnes *_x
corps = [col[:-2] for col in df.columns if col.endswith("_x")]
palette = ["yellow", "blue", "orange", "red", "green", "purple", "cyan", "magenta"]
couleurs = [palette[i % len(palette)] for i in range(len(corps))]

fig, ax = plt.subplots()
ax.set_aspect("equal")
ax.set_facecolor("black")
fig.patch.set_facecolor("black")

# Limites de la zone visible
xmin = min(df[f"{c}_x"].min() for c in corps)
xmax = max(df[f"{c}_x"].max() for c in corps)
ymin = min(df[f"{c}_y"].min() for c in corps)
ymax = max(df[f"{c}_y"].max() for c in corps)
marge = 0.05 * max(xmax - xmin, ymax - ymin)
ax.set_xlim(xmin - marge, xmax + marge)
ax.set_ylim(ymin - marge, ymax + marge)

# Trajectoires (trace estompée) et points mobiles
traces = [ax.plot([], [], "-", color=c, alpha=0.3, lw=0.8)[0] for c in couleurs]
points = [ax.plot([], [], "o", color=c, label=n)[0] for c, n in zip(couleurs, corps)]

ax.legend(facecolor="black", labelcolor="white", loc="upper right")

# On saute des frames pour accélérer l'animation
STEP = max(1, len(df) // 500)

def init():
    for t, p in zip(traces, points):
        t.set_data([], [])
        p.set_data([], [])
    return traces + points

def update(frame):
    i = frame * STEP
    for k, c in enumerate(corps):
        xs = df[f"{c}_x"].iloc[:i+1]
        ys = df[f"{c}_y"].iloc[:i+1]
        traces[k].set_data(xs, ys)
        points[k].set_data([xs.iloc[-1]], [ys.iloc[-1]])
    return traces + points

n_frames = len(df) // STEP
ani = animation.FuncAnimation(
    fig, update, frames=n_frames, init_func=init, interval=20, blit=True
)

plt.tight_layout()
plt.show()
