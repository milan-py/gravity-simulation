import pandas as pd
import plotly.graph_objects as go
from plotly.subplots import make_subplots

df = pd.read_csv("data.csv")
# df = df.melt(value_vars = ["mvelX", "mvelY", "posX", "posY", "distance"], id_vars = ["i"])



# Create figure with secondary y-axis
fig = make_subplots(specs=[[{"secondary_y": True}]])

# Add traces
fig.add_trace(
    go.Scatter(x = df["i"], y = df["mvelX"], name = "mvelx"),
    secondary_y = True,
)

fig.add_trace(
    go.Scatter(x = df["i"], y = df["mvelY"], name = "mvelY"),
    secondary_y = True,
)

fig.add_trace(
    go.Scatter(x = df["i"], y = df["posX"], name = "posX"),
)

fig.add_trace(
    go.Scatter(x = df["i"], y = df["posY"], name = "posY"),
)

fig.add_trace(
    go.Scatter(x = df["i"], y = df["distance"], name = "distance"),
)

fig.show()