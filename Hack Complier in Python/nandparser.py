from lexer import Tokenizer, OPERATOR_TOKEN

class Parser:
    A_COMMAND = 0
    C_COMMAND = 1
    L_COMMAND = 2
    
    def __init__(self, asm_file: str):
        self.tokenizer = Tokenizer(asm_file)
        self.reset_command_data()

    def reset_command_data(self):
        self.command_type = -1
        self.symbol_data = ''
        self.dest_data = ''
        self.comp_data = ''
        self.jump_data = ''

    def has_more_commands(self):
        return self.tokenizer.more_instructions()

    def advance(self):
        self.reset_command_data()
        self.tokenizer.fetch_next_instruction()
        token_type, token_value = self.tokenizer.present_token

        if token_type == OPERATOR_TOKEN:
            if token_value == '@':
                self.parse_a_command()
            elif token_value == '(':
                self.parse_l_command()
        else:
            self.parse_c_command(token_type, token_value)

    def parse_a_command(self):
        self.command_type = self.A_COMMAND
        _, self.symbol_data = self.tokenizer.load_next_token()

    def parse_l_command(self):
        self.command_type = self.L_COMMAND
        _, self.symbol_data = self.tokenizer.load_next_token()

    def parse_c_command(self, initial_token_type, initial_token_value):
        self.command_type = self.C_COMMAND
        comp_token_type, comp_token_value = self.extract_dest(initial_token_type, initial_token_value)
        self.extract_comp(comp_token_type, comp_token_value)
        self.extract_jump()

    def extract_dest(self, initial_token_type, initial_token_value):
        next_token_type, next_token_value = self.tokenizer.preview_next_token()
        
        if next_token_type == OPERATOR_TOKEN and next_token_value == '=':
            self.tokenizer.load_next_token()
            self.dest_data = initial_token_value
            comp_token_type, comp_token_value = self.tokenizer.load_next_token()
        else:
            comp_token_type, comp_token_value = initial_token_type, initial_token_value
        return comp_token_type, comp_token_value

    def extract_comp(self, token_type, token_value):
        if token_type == OPERATOR_TOKEN and (token_value == '-' or token_value == '!'):
            next_token_type, next_token_value = self.tokenizer.load_next_token()
            self.comp_data = token_value + next_token_value
        else:
            self.comp_data = token_value

            next_token_type, next_token_value = self.tokenizer.preview_next_token()
            if next_token_type == OPERATOR_TOKEN and next_token_value != ';':
                self.tokenizer.load_next_token()
                last_token_type, last_token_value = self.tokenizer.load_next_token()
                self.comp_data += next_token_value + last_token_value

    def extract_jump(self):
        token_type, token_value = self.tokenizer.load_next_token()
        
        if token_type == OPERATOR_TOKEN and token_value == ';':
            _, self.jump_data = self.tokenizer.load_next_token()

# Note: Import the Tokenizer class properly. The above code assumes that the Tokenizer class has been imported as shown.
