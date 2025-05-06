# Import necessary libraries
from sklearn.model_selection import train_test_split
from sklearn.tree import DecisionTreeClassifier
from micromlgen import port
import pandas as pd

# Load dataset
df = pd.read_csv("/Users/zhuoshangming/Desktop/515/lab3 Shangming/Source code with comments and dataset/src/Sorting_Hat_Training_Data.csv")

# Prepare dataset
X = df.drop(columns=["House"])  # Features
y = df["House"].astype('category').cat.codes  # Convert House names to numeric labels

# Split into training and testing sets
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)

# Train a Decision Tree model
model = DecisionTreeClassifier(max_depth=5, random_state=42)
model.fit(X_train, y_train)

# Evaluate model accuracy
accuracy = model.score(X_test, y_test)
print(f"Model Accuracy: {accuracy:.2f}")

# Convert trained model to C++ for ESP32
classmap = {
    0: 'Gryffindor',
    1: 'Ravenclaw',
    2: 'Hufflepuff',
    3: 'Slytherin'
}
c_code = port(model, classmap=classmap)

# Save C++ model to a file for ESP32 deployment
model_filename = "/Users/zhuoshangming/Desktop/515/lab3 Shangming/Source code with comments and dataset/sorting_hat_esp_button_64_width/sorting_hat_esp_button_64_width/sorting_hat_model.h"
with open(model_filename, "w") as f:
    f.write(c_code)

print(f"Model successfully converted and saved as {model_filename}!")
