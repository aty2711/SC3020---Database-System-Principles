import sys
from PyQt6 import QtWidgets

from interface import LoginWidget, ErrorDialog, MainUI

from explain import get_database_names, check_connection, LoginDetails, QueryDetails


class Main:
    def __init__(self):
        self.app = QtWidgets.QApplication(sys.argv)
        self.login_details = LoginDetails
        self.login_details.host = "localhost"
        self.login_details.port = "5432"
        self.login_details.user = "postgres"
        self.login_details.password = "no_password"
        self.login_details = (
            self.login()
        )  # Call login window, get user login_details on connect.

    def login(self):
        """
        Show login interface.
        """
        login_successful = False
        while not login_successful:
            loginpage = QtWidgets.QWidget()
            login_ui = LoginWidget(self.login_details)
            login_ui.loadUi(loginpage)
            loginpage.show()
            self.app.exec()  # Use the QApplication instance created in __init__

            if not check_connection(login_ui.login_details):
                continue
            else:
                login_successful = True

        return login_ui.login_details

    def load_main_page(self, db_list):
        """
        Start main program interface logic
        """
   

    def main(self):
        db_list = get_database_names(
            self.login_details
        )  # Function returns list of database names

        # Load main page of app
        main_ui = MainUI(
            self.login_details, db_list
        )  # Assign main_ui to instance of MainUI class

        main_ui.show()
        self.app.exec()

    # Standard error static method to be called throughout the 3 files
    @staticmethod
    def show_error(msg):
        """
        Show error dialog
        """

        error_dialog = ErrorDialog(msg)
        error_dialog.exec()  # This will display the dialog modally


if __name__ == "__main__":
    main = Main()
    main.main()
