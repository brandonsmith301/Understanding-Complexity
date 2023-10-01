class Generator:
    def __init__(self):
        self.dest_encoding = ['', 'M', 'D', 'MD', 'A', 'AM', 'AD', 'AMD']
        self.jump_encoding = ['', 'JGT', 'JEQ', 'JGE', 'JLT', 'JNE', 'JLE', 'JMP']
        self.comp_encoding = {
            '0': '0101010', '1': '0111111', '-1': '0111010', 'D': '0001100',
            'A': '0110000', '!D': '0001101', '!A': '0110001', '-D': '0001111',
            '-A': '0110011', 'D+1': '0011111', 'A+1': '0110111', 'D-1': '0001110',
            'A-1': '0110010', 'D+A': '0000010', 'D-A': '0010011', 'A-D': '0000111',
            'D&A': '0000000', 'D|A': '0010101',
            'M': '1110000', '!M': '1110001', '-M': '1110011',
            'M+1': '1110111', 'M-1': '1110010', 'D+M': '1000010',
            'D-M': '1010011', 'M-D': '1000111', 'D&M': '1000000', 'D|M': '1010101'
        }

    def generate_a(self, address):
        return '0' + self._convert_to_bits(address).zfill(15)

    def generate_c(self, destination, computation, jump_cond):
        return '111' + self.compute_comp(computation) + self.compute_dest(destination) + self.compute_jump(jump_cond)

    def compute_dest(self, destination_val):
        dest_index = self.dest_encoding.index(destination_val)
        return self._convert_to_bits(dest_index).zfill(3)

    def compute_comp(self, computation_val):
        return self.comp_encoding.get(computation_val, 'xxxxxxx')

    def compute_jump(self, jump_val):
        jump_index = self.jump_encoding.index(jump_val)
        return self._convert_to_bits(jump_index).zfill(3)

    def _convert_to_bits(self, num):
        return bin(int(num))[2:]
