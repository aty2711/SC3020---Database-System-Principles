from PyQt6 import QtCore, QtGui, QtWidgets
from PyQt6.QtWidgets import QMainWindow, QWidget, QHBoxLayout, QVBoxLayout, QPushButton, QTextEdit, QLabel, QComboBox, QTreeWidgetItem, QTreeWidget
from PyQt6.QtCore import Qt
import json

from explain import QueryDetails, LoginDetails, retrieve_query, load_qep_explanations, initialize_tree

class LoginWidget(object):
    def __init__(self, login_details):
        self.login_details = login_details

    def loadUi(self, loginWidget):
        # loginWidget.setObjectName("login")
        loginWidget.resize(500, 500)
        loginWidget.setStyleSheet("QWidget#login {background-color: #f0f0f0;}")
        loginWidget.setWindowTitle("Login")

        # Label for Host
        self.hostLabel = QtWidgets.QLabel(loginWidget)
        # self.hostLabel.setObjectName("hostLabel")
        font = QtGui.QFont()
        self.hostLabel.setFont(font)
        self.hostLabel.setStyleSheet('color: "#6a6b79";\n' "font: 12px")
        self.hostLabel.setText("Host")

        # Input Field for Host
        self.hostInput = QtWidgets.QLineEdit(loginWidget)
        # self.hostInput.setObjectName("hostInput")
        font = QtGui.QFont()
        self.hostInput.setFont(font)
        self.hostInput.setStyleSheet('color: "#018076";\n' "font: 12px")
        self.hostInput.setText(self.login_details.host)

        # Label for Username
        self.userLabel = QtWidgets.QLabel(loginWidget)
        # self.userLabel.setObjectName("userLabel")
        font = QtGui.QFont()
        self.userLabel.setFont(font)
        self.userLabel.setStyleSheet('color: "#6a6b79";\n' "font: 12px")
        self.userLabel.setText("Username")

        # Input Field for Username
        self.userInput = QtWidgets.QLineEdit(loginWidget)
        # self.userInput.setObjectName("userInput")
        font = QtGui.QFont()
        self.userInput.setFont(font)
        self.userInput.setStyleSheet('color: "#018076";\n' "font: 12px")
        self.userInput.setText(self.login_details.user)

        # # Label for Port
        self.portLabel = QtWidgets.QLabel(loginWidget)
        # self.portLabel.setObjectName("portLabel")
        font = QtGui.QFont()
        self.portLabel.setFont(font)
        self.portLabel.setStyleSheet('color: "#6a6b79";\n' "font: 12px")
        self.portLabel.setText("Port")

        # Input Field for Port
        self.portInput = QtWidgets.QLineEdit(loginWidget)
        # self.portInput.setObjectName("portInput")
        font = QtGui.QFont()
        self.portInput.setFont(font)
        self.portInput.setStyleSheet('color: "#018076";\n' "font: 12px")
        self.portInput.setText(str(self.login_details.port))

        # Label for Password
        self.passwordLabel = QtWidgets.QLabel(loginWidget)
        # self.passwordLabel.setObjectName("passwordLabel")
        font = QtGui.QFont()
        self.passwordLabel.setFont(font)
        self.passwordLabel.setStyleSheet('color: "#6a6b79";\n' "font: 12px")
        self.passwordLabel.setText("Password")

        # Input Field for Password
        self.passwordInput = QtWidgets.QLineEdit(loginWidget)
        # self.passwordInput.setObjectName("passwordInput")
        font = QtGui.QFont()
        self.passwordInput.setFont(font)
        self.passwordInput.setStyleSheet('color: "#018076";\n' "font: 12px")
        self.passwordInput.setText(self.login_details.password)
        self.passwordInput.setEchoMode(QtWidgets.QLineEdit.EchoMode.Password)

        # Login Button
        self.loginButton = QtWidgets.QPushButton(loginWidget)
        self.loginButton.setText("Login")
        font = QtGui.QFont()
        self.loginButton.setFont(font)
        self.loginButton.setStyleSheet(
            'background-color: "#004146";\n'
            "color: white;\n"
            "border-style: outset;\n"
            "border-radius: 10px;\n"
            "font: 12px"
        )

        # Layout
        self.gridLayout = QtWidgets.QGridLayout(loginWidget)
        self.gridLayout.addWidget(self.hostLabel, 0, 0, 1, 1)
        self.gridLayout.addWidget(self.hostInput, 0, 1, 1, 1)
        self.gridLayout.addWidget(self.userLabel, 1, 0, 1, 1)
        self.gridLayout.addWidget(self.userInput, 1, 1, 1, 1)
        self.gridLayout.addWidget(self.portLabel, 2, 0, 1, 1)
        self.gridLayout.addWidget(self.portInput, 2, 1, 1, 1)
        self.gridLayout.addWidget(self.passwordLabel, 3, 0, 1, 1)
        self.gridLayout.addWidget(self.passwordInput, 3, 1, 1, 1)
        self.gridLayout.addWidget(self.loginButton, 4, 0, 1, 2)

        # Signals to connect listener to event handler
        self.loginButton.clicked.connect(self.onClickLogin)
        self.loginButton.clicked.connect(loginWidget.close)

    def onClickLogin(self):
        self.login_details.host = self.hostInput.text()
        self.login_details.port = self.portInput.text()
        self.login_details.user = self.userInput.text()
        self.login_details.password = self.passwordInput.text()

class MainUI(QMainWindow):
    def __init__(self, login_details, db_list):
        super().__init__()
        self.login_details = login_details
        self.db_list = db_list
        self.tree_widget = None #  QTree instance
        self.qep_tree = None # Tree instance

        self.setWindowTitle("SQL Query Executor")
        self.resize(1350, 882)
        self.setStyleSheet("background-color: #ffffff;")

        # Central widget and main horizontal layout
        central_widget = QWidget()
        self.setCentralWidget(central_widget)
        main_layout = QHBoxLayout()
        central_widget.setLayout(main_layout)

        # Left column for database selection, SQL Query input, and execute button
        left_layout = QVBoxLayout()
        
        # Database Selection Dropdown
        self.database_selector = QComboBox()
        self.database_selector.addItems(self.db_list)  # Populate dropdown with database names
        self.database_selector.setCurrentIndex(-1)  # Optionally set no selection by default
        self.database_selector.setPlaceholderText("Select Database")
        left_layout.addWidget(self.database_selector)

        # default selection
        if("TPC-H" in self.db_list):
            index = self.database_selector.findText("TPC-H")
            self.database_selector.setCurrentIndex(index)
        else:
            self.database_selector.setCurrentIndex(0)

        
        # SQL Query Input
        self.sql_input = QtWidgets.QPlainTextEdit()
        self.sql_input.setPlaceholderText("Enter SQL query here...")
        self.sql_input.setMinimumHeight(100)  # Make input field larger
        left_layout.addWidget(self.sql_input)
        self.sql_input.setPlainText("SELECT * FROM region LEFT JOIN nation on region.r_regionkey = nation.n_regionkey WHERE n_regionkey = 1 ORDER BY r_name DESC")

        # Execute Query Button
        self.execute_button = QPushButton("Execute Query")
        left_layout.addWidget(self.execute_button)
        
        # Container for left layout
        left_widget = QWidget()
        left_widget.setLayout(left_layout)
        main_layout.addWidget(left_widget, 1)  # Adjust stretch factor as needed

        # Middle column for SQL Query output
        self.query_output = QTextEdit()
        self.query_output.setReadOnly(True)
        self.query_output.setPlaceholderText("Query output will be displayed here...")
        main_layout.addWidget(self.query_output, 2)  # Adjust stretch factor as needed

        # Right column for File Tree / Query Execution Plan
        right_layout = QVBoxLayout()
        right_layout.addWidget(QLabel("QEP File Tree:"))
        
        self.tree_widget = QTreeWidget()
        self.tree_widget.setHeaderLabels(["Node Type", "Total Cost"])
        right_layout.addWidget(self.tree_widget)
     
        # Add right layout to a container widget and then to the main layout
        right_widget = QWidget()
        right_widget.setLayout(right_layout)
        main_layout.addWidget(right_widget, 1)  # Adjust stretch factor as needed

        # Connect the button click to a method (to be implemented)
        self.execute_button.clicked.connect(lambda: self.execute_query(self.database_selector.currentText(), self.sql_input.toPlainText()))

    def execute_query(self, database_name, query):
        self.tree_widget.clear()
        query_details = QueryDetails
        query_details.database = database_name
        query_details.query = query
        qep = retrieve_query(self.login_details, query_details)
        self.query_output.setText(json.dumps(qep[0][0][0], indent=4))

        self.qep_tree = initialize_tree(qep[0][0][0]['Plan'], self.login_details, query_details)

        self.populate_tree_widget(self.qep_tree)
        
        # Resize columns to fit content
        self.tree_widget.setColumnWidth(0,200)
        self.tree_widget.setColumnWidth(1,100)

        # Append explanations into output field
        explanations = load_qep_explanations(self.qep_tree)
        self.append_query_output(explanations)
        
    def append_query_output(self, textual_query):
        self.query_output.append("\n--------------------------------------------")
        self.query_output.append(textual_query)
        self.query_output.append("----------------------------------------------")

    def build_tree_recursive(self, node, parent_widget):
        if node is not None:
            tree_item = QTreeWidgetItem(parent_widget, [f"{node.id}. {node.node_json["Node Type"]}", str(node.node_json["Total Cost"])])
            self.build_tree_recursive(node.left, tree_item)        
            self.build_tree_recursive(node.right, tree_item)

    def populate_tree_widget(self, tree):
        root = tree.root
        tree_item = QTreeWidgetItem(self.tree_widget, [f"{root.id}. {root.node_json["Node Type"]}", str(tree.root.node_json["Total Cost"])])
        self.build_tree_recursive(tree.root.left, tree_item)
        self.build_tree_recursive(tree.root.right, tree_item)

class ErrorDialog(QtWidgets.QDialog):
    def __init__(self, message, parent=None):
        super().__init__(parent)
        self.setWindowTitle("Error")
        self.resize(410, 200)  # Adjust size as needed
        self.setStyleSheet("background-color: #ffffff;")

        # Setup scroll area
        self.scrollArea = QtWidgets.QScrollArea(self)
        self.scrollArea.setGeometry(
            QtCore.QRect(30, 30, 350, 100)
        )  # Adjust geometry as needed
        self.scrollArea.setWidgetResizable(True)
        self.scrollAreaWidgetContents = QtWidgets.QWidget()
        self.scrollAreaWidgetContents.setGeometry(
            QtCore.QRect(0, 0, 348, 98)
        )  # Adjust geometry as needed

        # Error Label within scroll area
        self.errorLabel = QtWidgets.QLabel(self.scrollAreaWidgetContents)
        self.errorLabel.setStyleSheet('color: "#018076"; font: 12px')
        self.errorLabel.setText(message)
        self.errorLabel.setWordWrap(True)
        self.errorLabel.adjustSize()  # Adjust the label size to fit the text
        self.scrollArea.setWidget(self.scrollAreaWidgetContents)

        # Warn Label
        self.warnLabel = QtWidgets.QLabel("Error:", self)
        self.warnLabel.setGeometry(QtCore.QRect(30, 10, 61, 16))
        self.warnLabel.setStyleSheet('color: "#6a6b79"; font: 11px')

        # Acknowledge Button
        self.backButton = QtWidgets.QPushButton("Acknowledge", self)
        self.backButton.setGeometry(
            QtCore.QRect(135, 140, 140, 31)
        )  # Adjust position as needed
        self.backButton.setStyleSheet(
            'background-color: "#004146"; color: white; '
            "border-style: outset; border-radius: 10px; font: 12px"
        )
        self.backButton.clicked.connect(
            self.accept
        )  # Use accept() for OK button behavior
