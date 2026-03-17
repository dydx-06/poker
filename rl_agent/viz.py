import subprocess
import pandas as pd
import matplotlib.pyplot as plt
import io

def run_simulation():
    result = subprocess.run(['./poker_test'], capture_output=True, text=True)
    csv_data = io.StringIO(result.stdout)
    df = pd.read_csv(csv_data)
    
    total_hands = df['Frequency'].sum()
    df['Probability (%)'] = (df['Frequency'] / total_hands) * 100
    
    return df

def plot_distribution(df):
    plt.figure(figsize=(12, 6))
    
    bars = plt.bar(df['Hand'], df['Probability (%)'], color='skyblue', edgecolor='black')
    
    for bar in bars:
        yval = bar.get_height()
        plt.text(bar.get_x() + bar.get_width()/2, yval * 1.3, 
                 f'{yval:.3f}%', ha='center', va='bottom', fontsize=9)

    plt.ylabel('Probability (%)', fontsize=12)
    plt.xlabel('Hand Rank', fontsize=12)
    plt.xticks(rotation=45, ha='right')
    plt.yscale('log')
    plt.tight_layout()
    
    plt.show()

if __name__ == "__main__":
    data = run_simulation()
    print(data.to_string(index=False))
    plot_distribution(data)