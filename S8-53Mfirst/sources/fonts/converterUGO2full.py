import os
from struct import *

def WriteByte(num_byte, file):
    value = int(symbols[num_byte])
    if value < 0:
        value += 256
    file.write(str(value))

def WriteReverseByte(num_byte, file):
    value = int(symbols[num_byte])
    if value < 0:
        value += 256
    revValue = 0
    base = 256
    while value != 0:
        revValue += (value & 1) * (base >> 1)
        base = base >> 1
        value >>= 1
    file.write(str(revValue))

def GetBit(byte, bit):
    if((byte & (1 << bit)) > 0):
        return 1
    return 0

def GetWidthByte(byte):
    retValue = 8
    for bit in range(8):
        if(GetBit(byte, bit) == 1):
            return retValue
        retValue -= 1
    return retValue

def CalculateWidth(num_symbol):
    retValue = 0;
    for num_byte in range(8):
        byte = symbols[num_symbol * 8 + num_byte]
        if GetWidthByte(byte) > retValue:
            retValue = GetWidthByte(byte)
    return retValue

###### Start here ######
input = open("FontUGO2.bin", "rb")
data = input.read()
input.close()
symbols = unpack("2048b", data)

output = open("fontUGO2.inc", "w")
output.write("// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by\n\n")
output.write("#include \"common/Display/Font/Font_.h\"\n\nconst Font fontUGO2 = {\n\t8, {\n")

outputDisplay = open("fontUGO2display.inc", "w")
outputDisplay.write("// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by\n\n")
outputDisplay.write("const uint8 fontUGO2display[3080] = {\n")
outputDisplay.write("/* Orient = 0x00, FontID = 0x00 */ 0x00, 0x00,\n")
outputDisplay.write("/* First character ID */           0x00, 0x00,\n")
outputDisplay.write("/* Last  character ID */           0xff, 0xff,\n")
outputDisplay.write("/* Height */                       0x08, 0x00,\n")

outputInterface = open("fontUGO2.cs", "w")             # Этот шрифт будет использоваться в клиентской программе на PC
outputInterface.write("namespace ControlLibraryS8_53\n")
outputInterface.write("{\n")
outputInterface.write("\tpartial class Display\n")
outputInterface.write("\t{\n")
outputInterface.write("\t\tprivate void InitFontUGO2()\n")
outputInterface.write("\t\t{\n")
outputInterface.write("\t\t\tfonts[3] = new MyFont();\n")
outputInterface.write("\t\t\tMyFont font = fonts[3];\n")
outputInterface.write("\t\t\tfont.height = 8;\n")

outTmpTable = open("table.map", "w")
outTmpMap = open("map.tmp", "w")

for num_symbol in range(256):
    output.write("/*" + str(num_symbol) + "*/")
    outputInterface.write("\t\t\tfont.symbols[" + str(num_symbol) + "] = new Symbol(")
    outTmpTable.write("\n/* " + str(num_symbol) + " */\t        ")
    
    output.write("\t\t{ " + str(CalculateWidth(num_symbol)) + ", { ")
    outputInterface.write(str(CalculateWidth(num_symbol)) + ", new int[8] { ")
    outTmpTable.write(str(CalculateWidth(num_symbol)) + ", ")
    offset = 8 + 4 * 256 + num_symbol * 8
    outTmpTable.write(str(offset & 0xff) + ", " + str((offset >> 8) & 0xff) + ", " + str((offset >> 16) & 0xff) + ",")

    outTmpMap.write("/* " + str(num_symbol) + " */\t       ")
    
    for num_byte in range(8):
        WriteByte(num_symbol * 8 + num_byte, output)
        WriteByte(num_symbol * 8 + num_byte, outputInterface)
        WriteReverseByte(num_symbol * 8 + num_byte, outTmpMap)
        if not (num_byte == 7 and num_symbol == 255):
            outTmpMap.write(",")
        if num_byte != 7:
            outputInterface.write(",")
            output.write(",") 
        output.write("\t")
        outputInterface.write(" ")

    outTmpMap.write("\n")
    
    output.write("} " + "}")
    outputInterface.write(" });\n")
    if num_symbol != 255:
        output.write(",")
    output.write("\t\n")
output.write("} };\n")

outTmpMap.write("};\n")

outputDisplay.write("\n/* Table of characters */\n")
outTmpTable.close()
outTmpTable = open("table.map")
lines = outTmpTable.readlines()
for line in lines:
    outputDisplay.write(line)

outputDisplay.write("\n\n/* Map of characters */\n\n")
outTmpMap.close()
outTmpMap = open("map.tmp")
lines = outTmpMap.readlines()
for line in lines:
    outputDisplay.write(line)

output.close()
outputDisplay.close();

outputInterface.write("}\n")
outputInterface.write("}\n")
outputInterface.write("}\n")
outputInterface.close();

input = open("fontUGO.inc")
#print(input.read())
input.close()
