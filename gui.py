import tkinter as tk
from encryption import encrypt


window = tk.Tk()

entry = tk.Entry(
    width=20,
)

entry.pack()

button = tk.Button(
    text="Add a Tag",
    width=25,
    height=5,
    bg="grey",
    fg = 'black',
    command = encrypt("abcdefghijklmnop",entry.get())
)

button.pack()

window.mainloop()
