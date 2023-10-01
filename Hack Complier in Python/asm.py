import subprocess

try:
    import fuzzywuzzy
except ImportError:
    subprocess.run(["pip", "install", "fuzzywuzzy"])
    import fuzzywuzzy

import sys
from nandparser import Parser
from generator import Generator
from symboltable import Symbol

class Assembler:
    def __init__(self):
        self.symbols = Symbol()
        self.next_available_address = 16

    def first_pass(self, file_path):
        parser = Parser(file_path)
        current_address = 0
        while parser.has_more_commands():
            parser.advance()
            cmd_type = parser.command_type
            if cmd_type in [Parser.A_COMMAND, Parser.C_COMMAND]:
                current_address += 1
            elif cmd_type == Parser.L_COMMAND:
                self.symbols.add_entry(parser.symbol_data, current_address)

    def second_pass(self, input_file, output_file):
        parser = Parser(input_file)
        with open(output_file, 'w') as out_file:
            code_gen = Generator()
            while parser.has_more_commands():
                parser.advance()
                cmd_type = parser.command_type
                if cmd_type == Parser.A_COMMAND:
                    out_file.write(code_gen.generate_a(self._resolve_address(parser.symbol_data)) + '\n')
                elif cmd_type == Parser.C_COMMAND:
                    out_file.write(code_gen.generate_c(parser.dest_data, parser.comp_data, parser.jump_data) + '\n')

    def _resolve_address(self, symbol):
        if symbol.isdigit():
            return symbol
        elif not self.symbols.contains(symbol):
            self.symbols.add_entry(symbol, self.next_available_address)
            self.next_available_address += 1
        return str(self.symbols.get_address(symbol))

    def assemble(self, file_path):
        self.first_pass(file_path)
        output_path = self._get_output_file_name(file_path)
        self.second_pass(file_path, output_path)

    @staticmethod
    def _get_output_file_name(input_file):
        return input_file.replace('.asm', '.hack') if input_file.endswith('.asm') else input_file + '.hack'


def main():
    if len(sys.argv) != 2:
        print("Usage: Assembler file.asm")
        return 

    input_file_path = sys.argv[1]
    assembler = Assembler()
    assembler.assemble(input_file_path)

if __name__ == "__main__":
    main()
