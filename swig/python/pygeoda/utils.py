try:
    import matplotlib 
except ImportError:
    print("Matplotlib is not found. Please install Matplotlib first.")


lisa_colors = {0:'#eeeeee', 1 : '#FF0000', 2 : '#0000FF', 3:'#a7adf9', 4 :'#f4ada8', 5 :'#999999'}
lisa_labels = {0:'Not significant', 1:'High-High', 2:'Low-Low', 3:'Low-High', 4:'High-Low', 5:'Neighborless'}
lisa_legend = [matplotlib.lines.Line2D([0], [0], color=color, lw=2) for color in lisa_colors.values()]