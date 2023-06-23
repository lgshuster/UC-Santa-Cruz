import tkinter as tk
window = tk.Tk()
label = tk.Label(window, text="0")
label.pack()

def inc():
    count = int(label["text"])
    label["text"] = str(count + 1)

button = tk.Button(window, text="+1", command=inc)
button.pack()
window.mainloop()
