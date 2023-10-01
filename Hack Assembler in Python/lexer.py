from fuzzywuzzy import process
import re

NUMBER_TOKEN = 1
IDENTIFIER_TOKEN = 2
OPERATOR_TOKEN = 3
INVALID_TOKEN = 4

KNOWN_TOKENS = ["if", "else", "for", "while", "=;", "()", "@+", "&|!"]

class Tokenizer:
    def __init__(self, asm_file: str):
        self.comment_re = re.compile('//.*$')
        self.token_re = re.compile(r'\d+|\w+|[=;()@+\-&|!]')

        try:
            with open(asm_file, 'r') as file_obj:
                raw_source = file_obj.read()
        except FileNotFoundError:
            raise Exception(f"The file {asm_file} was not found.")
        
        self.source_lines = raw_source.split('\n')
        self.token_list = self.extract_tokens()
        self.current_instr = []
        self.present_token = (INVALID_TOKEN, 0)
        
    def more_instructions(self):
        return bool(self.token_list)
        
    def fetch_next_instruction(self):
        self.current_instr = self.token_list.pop(0)
        self.load_next_token()
        return self.current_instr

    def has_more_tokens(self):
        return bool(self.current_instr)

    def load_next_token(self):
        self.present_token = self.current_instr.pop(0) if self.current_instr else (INVALID_TOKEN, 0)
        if self.present_token[0] == INVALID_TOKEN:
            corrected_token = self.correct_token(self.present_token[1])
            self.present_token = (IDENTIFIER_TOKEN, corrected_token)
        return self.present_token

    def preview_next_token(self):
        next_token = self.current_instr[0] if self.current_instr else (INVALID_TOKEN, 0)
        if next_token[0] == INVALID_TOKEN:
            corrected_token = self.correct_token(next_token[1])
            next_token = (IDENTIFIER_TOKEN, corrected_token)
        return next_token

    def extract_tokens(self):
        return [line_token for line_token in (self.tokenize_single_line(l) for l in self.source_lines) if line_token]

    def tokenize_single_line(self, asm_line):
        return [self.categorize_token(tk) for tk in self.split_tokens(self.strip_comment(asm_line))]

    def strip_comment(self, asm_line):
        return self.comment_re.sub('', asm_line)

    def split_tokens(self, asm_line):
        return self.token_re.findall(asm_line)
        
    def categorize_token(self, tk):
        if re.match(r'\d+', tk):
            return NUMBER_TOKEN, tk
        elif re.match(r'\w+', tk):
            return IDENTIFIER_TOKEN, tk
        elif re.match(r'[=;()@+\-&|!]', tk):
            return OPERATOR_TOKEN, tk
        else:
            return INVALID_TOKEN, tk

    def correct_token(self, invalid_token):
        invalid_token = str(invalid_token)  
        best_match, _ = process.extractOne(invalid_token, KNOWN_TOKENS)
        return best_match if best_match else invalid_token

