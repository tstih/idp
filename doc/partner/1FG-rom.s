--------------------------------------------------
Z80 DISASSEMBLER LISTING
Line   Addr Opcodes     Label   Instruction
--------------------------------------------------

0001   0000 C3 6A 01            JP L0001
0002   0003 31 C0 FF    L0006:  LD SP,0FFC0H
0003   0006 CD BC 00            CALL L0002
0004   0009 3E 2A               LD A,2AH
0005   000B CD A9 00            CALL L0003
0006   000E CD 9F 00            CALL L0004
0007   0011 E6 DF               AND 0DFH
0008   0013 FE 52               CP 52H
0009   0015 CA B2 02            JP Z,L0005
0010   0018 FE 46               CP 46H
0011   001A CA B2 02            JP Z,L0005
0012   001D 3E 3F       L0007:  LD A,3FH
0013   001F CD A9 00            CALL L0003
0014   0022 18 DF               JR L0006
0015   0024 00                  NOP
0016   0025 00                  NOP
0017   0026 00                  NOP
0018   0027 00                  NOP
0019   0028 00                  NOP
0020   0029 00                  NOP
0021   002A 00                  NOP
0022   002B 00                  NOP
0023   002C 00                  NOP
0024   002D 00                  NOP
0025   002E 00                  NOP
0026   002F 00                  NOP
0027   0030 00                  NOP
0028   0031 00                  NOP
0029   0032 00                  NOP
0030   0033 00                  NOP
0031   0034 00                  NOP
0032   0035 00                  NOP
0033   0036 00                  NOP
0034   0037 00                  NOP
0035   0038 00                  NOP
0036   0039 00                  NOP
0037   003A 00                  NOP
0038   003B 00                  NOP
0039   003C 00                  NOP
0040   003D 00                  NOP
0041   003E 00                  NOP
0042   003F 00                  NOP
0043   0040 00                  NOP
0044   0041 00                  NOP
0045   0042 00                  NOP
0046   0043 00                  NOP
0047   0044 00                  NOP
0048   0045 00                  NOP
0049   0046 00                  NOP
0050   0047 00                  NOP
0051   0048 00                  NOP
0052   0049 00                  NOP
0053   004A 00                  NOP
0054   004B 00                  NOP
0055   004C 00                  NOP
0056   004D 00                  NOP
0057   004E 00                  NOP
0058   004F 00                  NOP
0059   0050 00                  NOP
0060   0051 00                  NOP
0061   0052 00                  NOP
0062   0053 00                  NOP
0063   0054 00                  NOP
0064   0055 00                  NOP
0065   0056 00                  NOP
0066   0057 00                  NOP
0067   0058 00                  NOP
0068   0059 00                  NOP
0069   005A 00                  NOP
0070   005B 00                  NOP
0071   005C 00                  NOP
0072   005D 00                  NOP
0073   005E 00                  NOP
0074   005F 00                  NOP
0075   0060 00                  NOP
0076   0061 00                  NOP
0077   0062 00                  NOP
0078   0063 00                  NOP
0079   0064 00                  NOP
0080   0065 00                  NOP
0081   0066 C3 1D 00            JP L0007
0082   0069 CD BC 00            CALL L0002
0083   006C 7C                  LD A,H
0084   006D CD 79 00            CALL L0008
0085   0070 7D                  LD A,L
0086   0071 CD 79 00            CALL L0008
0087   0074 3E 20               LD A,20H
0088   0076 C3 A9 00            JP L0003
0089   0079 C5          L0008:  PUSH BC
0090   007A 47                  LD B,A
0091   007B CB 2F               SRA A
0092   007D CB 2F               SRA A
0093   007F CB 2F               SRA A
0094   0081 CB 2F               SRA A
0095   0083 E6 0F               AND 0FH
0096   0085 CD 90 00            CALL L0009
0097   0088 78                  LD A,B
0098   0089 E6 0F               AND 0FH
0099   008B CD 90 00            CALL L0009
0100   008E C1                  POP BC
0101   008F C9                  RET
0102   0090 FE 0A       L0009:  CP 0AH
0103   0092 FA 99 00            JP M,L0010
0104   0095 C6 37               ADD A,37H
0105   0097 18 02               JR L0011
0106   0099 C6 30       L0010:  ADD A,30H
0107   009B CD A9 00    L0011:  CALL L0003
0108   009E C9                  RET
0109   009F DB D9       L0004:  IN A,(0D9H)
0110   00A1 CB 47               BIT 0,A
0111   00A3 28 FA               JR Z,L0004
0112   00A5 DB D8               IN A,(0D8H)
0113   00A7 CB BF               RES 7,A
-- SYNC --
0114   00A9 CD AF 00    L0003:  CALL L0012
0115   00AC D3 20               OUT (20H),A
0116   00AE C9                  RET
0117   00AF F5          L0012:  PUSH AF
0118   00B0 DB 20       L0013:  IN A,(20H)
0119   00B2 E6 04               AND 04H
0120   00B4 28 FA               JR Z,L0013
0121   00B6 F1                  POP AF
0122   00B7 C9                  RET
0123   00B8 3E 20               LD A,20H
0124   00BA 18 ED               JR L0003
0125   00BC CD AF 00    L0002:  CALL L0012
0126   00BF 3A EB FF            LD A,(0FFEBH)
0127   00C2 D6 0C               SUB 0CH
0128   00C4 32 EB FF            LD (0FFEBH),A
0129   00C7 D3 36               OUT (36H),A
0130   00C9 E5                  PUSH HL
0131   00CA 21 DB 00            LD HL,00DBH
0132   00CD CD FD 01            CALL L0014
0133   00D0 21 F0 00            LD HL,00F0H
0134   00D3 CD FD 01            CALL L0014
0135   00D6 E1                  POP HL
0136   00D7 AF                  XOR A
0137   00D8 C3 A9 00            JP L0003
0138   00DB 03                  INC BC
0139   00DC 00                  NOP
0140   00DD 05                  DEC B
0141   00DE 01 0B 0B            LD BC,0B0BH
0142   00E1 0B                  DEC BC
0143   00E2 0B                  DEC BC
0144   00E3 0B                  DEC BC
0145   00E4 0B                  DEC BC
0146   00E5 0B                  DEC BC
0147   00E6 0B                  DEC BC
0148   00E7 0B                  DEC BC
0149   00E8 0B                  DEC BC
0150   00E9 0B                  DEC BC
0151   00EA 0B                  DEC BC
0152   00EB 0B                  DEC BC
0153   00EC 0B                  DEC BC
0154   00ED 0B                  DEC BC
0155   00EE 0B                  DEC BC
0156   00EF 00                  NOP
0157   00F0 21 00 0D            LD HL,0D00H
0158   00F3 00                  NOP
0159   00F4 21 88 02    L0027:  LD HL,0288H
0160   00F7 CD FA 01            CALL L0015
0161   00FA 21 00 20            LD HL,2000H
0162   00FD 01 80 FF            LD BC,0FF80H
0163   0100 16 02               LD D,02H
0164   0102 3E 00               LD A,00H
0165   0104 D5          L0017:  PUSH DE
0166   0105 F5                  PUSH AF
0167   0106 CD 11 01            CALL L0016
0168   0109 F1                  POP AF
0169   010A D1                  POP DE
0170   010B C6 55               ADD A,55H
0171   010D 15                  DEC D
0172   010E 20 F4               JR NZ,L0017
0173   0110 C9                  RET
0174   0111 CD 22 01    L0016:  CALL L0018
0175   0114 D3 90               OUT (90H),A
0176   0116 CD 22 01            CALL L0018
0177   0119 CD 35 01            CALL L0019
0178   011C D3 88               OUT (88H),A
0179   011E CD 35 01            CALL L0019
0180   0121 C9                  RET
0181   0122 F5          L0018:  PUSH AF
0182   0123 E5                  PUSH HL
0183   0124 57                  LD D,A
0184   0125 2F                  CPL
0185   0126 5F                  LD E,A
0186   0127 72          L0020:  LD (HL),D
0187   0128 23                  INC HL
0188   0129 73                  LD (HL),E
0189   012A 23                  INC HL
0190   012B E5                  PUSH HL
0191   012C B7                  OR A
0192   012D ED 42               SBC HL,BC
0193   012F E1                  POP HL
0194   0130 38 F5               JR C,L0020
-- SYNC --
0195   0132 E1                  POP HL
0196   0133 F1                  POP AF
0197   0134 C9                  RET
0198   0135 E5          L0019:  PUSH HL
0199   0136 F5                  PUSH AF
0200   0137 57                  LD D,A
0201   0138 2F                  CPL
0202   0139 5F                  LD E,A
0203   013A 7E          L0022:  LD A,(HL)
0204   013B BA                  CP D
0205   013C 20 10               JR NZ,L0021
0206   013E 23                  INC HL
0207   013F 7E                  LD A,(HL)
0208   0140 BB                  CP E
0209   0141 20 0B               JR NZ,L0021
0210   0143 23                  INC HL
0211   0144 E5                  PUSH HL
0212   0145 B7                  OR A
0213   0146 ED 42               SBC HL,BC
0214   0148 E1                  POP HL
0215   0149 38 EF               JR C,L0022
0216   014B F1                  POP AF
0217   014C E1                  POP HL
0218   014D C9                  RET
0219   014E 21 57 01    L0021:  LD HL,0157H
0220   0151 CD FA 01            CALL L0015
0221   0154 C3 03 00            JP L0006
0222   0157 21 00 4D            LD HL,4D00H
0223   015A 45                  LD B,L
0224   015B 4D                  LD C,L
0225   015C 4F                  LD C,A
0226   015D 52                  LD D,D
0227   015E 59                  LD E,C
0228   015F 20 45               JR NZ,L0023
0229   0161 52                  LD D,D
0230   0162 52                  LD D,D
0231   0163 4F                  LD C,A
0232   0164 52                  LD D,D
0233   0165 20 21               JR NZ,L0024
0234   0167 21 21 00            LD HL,0021H
0235   016A 31 C0 FF    L0001:  LD SP,0FFC0H
0236   016D 3E 07               LD A,07H
0237   016F D3 31               OUT (31H),A
0238   0171 D3 33               OUT (33H),A
0239   0173 3E 0F               LD A,0FH
0240   0175 D3 31               OUT (31H),A
0241   0177 D3 33               OUT (33H),A
0242   0179 3E 18               LD A,18H
0243   017B D3 30               OUT (30H),A
0244   017D 3E 6D               LD A,6DH
0245   017F D3 32               OUT (32H),A
0246   0181 AF                  XOR A
0247   0182 D3 39               OUT (39H),A
0248   0184 D3 36               OUT (36H),A
0249   0186 32 EB FF            LD (0FFEBH),A
0250   0189 AF                  XOR A
0251   018A D3 21               OUT (21H),A
0252   018C 3E 03               LD A,03H
0253   018E D3 21               OUT (21H),A
0254   0190 3E 04               LD A,04H
0255   0192 CD A9 00            CALL L0003
0256   0195 3E 05               LD A,05H
0257   0197 CD A9 00            CALL L0003
0258   019A AF                  XOR A
0259   019B D3 39               OUT (39H),A
0260   019D D3 30               OUT (30H),A
0261   019F CD 19 02            CALL L0025
0262   01A2 CD 3D 02            CALL L0026
0263   01A5 0E D9               LD C,0D9H
0264   01A7 21 6D 02            LD HL,026DH
0265   01AA 06 07               LD B,07H
0266   01AC ED B3               OTIR
0267   01AE 0E DB               LD C,0DBH
0268   01B0 21 6D 02            LD HL,026DH
0269   01B3 06 07               LD B,07H
0270   01B5 ED B3               OTIR
0271   01B7 0E E1               LD C,0E1H
0272   01B9 21 6D 02            LD HL,026DH
0273   01BC 06 07               LD B,07H
0274   01BE ED B3               OTIR
0275   01C0 31 C0 FF            LD SP,0FFC0H
0276   01C3 3E 64               LD A,64H
0277   01C5 D3 2B               OUT (2BH),A
0278   01C7 21 74 02            LD HL,0274H
0279   01CA CD FA 01            CALL L0015
0280   01CD 3E 05               LD A,05H
0281   01CF CD A9 00            CALL L0003
0282   01D2 21 E4 01            LD HL,01E4H
0283   01D5 CD FA 01            CALL L0015
0284   01D8 CD F4 00            CALL L0027
0285   01DB CD B5 02            CALL L0028
0286   01DE CD A6 02            CALL L0029
0287   01E1 C3 B2 02            JP L0005
0288   01E4 21 00 5B            LD HL,5B00H
0289   01E7 20 42               JR NZ,L0030
0290   01E9 6F                  LD L,A
0291   01EA 6F                  LD L,A
0292   01EB 74                  LD (HL),H
0293   01EC 20 56               JR NZ,L0031
0294   01EE 20 31               JR NZ,L0032
0295   01F0 2E 30               LD L,30H
0296   01F2 20 2D               JR NZ,L0032
0297   01F4 20 31               JR NZ,L0033
0298   01F6 46                  LD B,(HL)
0299   01F7 20 5D               JR NZ,L0034
0300   01F9 00                  NOP
0301   01FA CD BC 00    L0015:  CALL L0002
0302   01FD CD AF 00    L0014:  CALL L0012
0303   0200 7E                  LD A,(HL)
0304   0201 D3 23               OUT (23H),A
0305   0203 23                  INC HL
0306   0204 7E                  LD A,(HL)
0307   0205 D3 22               OUT (22H),A
0308   0207 23                  INC HL
0309   0208 7E          L0035:  LD A,(HL)
0310   0209 B7                  OR A
0311   020A C8                  RET Z
0312   020B CD A9 00            CALL L0003
0313   020E 23                  INC HL
0314   020F 18 F7               JR L0035
0315   0211 C5          L0037:  PUSH BC
0316   0212 06 FF               LD B,0FFH
0317   0214 00          L0036:  NOP
0318   0215 10 FD               DJNZ L0036
0319   0217 C1                  POP BC
0320   0218 C9                  RET
0321   0219 3E 00       L0025:  LD A,00H
0322   021B D3 39               OUT (39H),A
0323   021D CD 11 02            CALL L0037
0324   0220 CD 11 02            CALL L0037
0325   0223 CD 11 02            CALL L0037
0326   0226 21 63 02            LD HL,0263H
0327   0229 AF                  XOR A
0328   022A D3 3E               OUT (3EH),A
0329   022C D3 3F               OUT (3FH),A
0330   022E D3 3A               OUT (3AH),A
0331   0230 D3 3B               OUT (3BH),A
0332   0232 3E 10               LD A,10H
0333   0234 D3 39               OUT (39H),A
0334   0236 06 0A               LD B,0AH
0335   0238 0E 38               LD C,38H
0336   023A ED B3               OTIR
0337   023C C9                  RET
0338   023D 3E 3D       L0026:  LD A,3DH
0339   023F D3 39               OUT (39H),A
0340   0241 AF                  XOR A
0341   0242 D3 3D               OUT (3DH),A
0342   0244 D3 3C       L0031:  OUT (3CH),A
0343   0246 21 FF 1F            LD HL,1FFFH
0344   0249 CD 58 02            CALL L0038
0345   024C 3E 20               LD A,20H
0346   024E D3 34               OUT (34H),A
0347   0250 AF                  XOR A
0348   0251 D3 35               OUT (35H),A
0349   0253 3E BB               LD A,0BBH
0350   0255 D3 39               OUT (39H),A
0351   0257 C9                  RET
0352   0258 3E 1A       L0038:  LD A,1AH
0353   025A D3 39               OUT (39H),A
0354   025C 7D                  LD A,L
0355   025D D3 38               OUT (38H),A
0356   025F 7C                  LD A,H
0357   0260 D3 38               OUT (38H),A
0358   0262 C9                  RET
0359   0263 D0                  RET NC
0360   0264 2F                  CPL
0361   0265 0D                  DEC C
0362   0266 05                  DEC B
0363   0267 99                  SBC A,C
0364   0268 4F                  LD C,A
0365   0269 0A                  LD A,(BC)
0366   026A EA 00 30            JP PE,L0039
0367   026D 18 04               JR L0040
0368   026F 44                  LD B,H
0369   0270 03                  INC BC
0370   0271 C1                  POP BC
0371   0272 05                  DEC B
0372   0273 68          L0040:  LD L,B
0373   0274 A8                  XOR B
0374   0275 00                  NOP
0375   0276 44                  LD B,H
0376   0277 65                  LD H,L
0377   0278 6C                  LD L,H
0378   0279 74                  LD (HL),H
0379   027A 61                  LD H,C
0380   027B 20 50               JR NZ,L0041
0381   027D 61                  LD H,C
0382   027E 72                  LD (HL),D
0383   027F 74                  LD (HL),H
0384   0280 6E                  LD L,(HL)
0385   0281 65                  LD H,L
0386   0282 72                  LD (HL),D
0387   0283 20 47               JR NZ,L0042
0388   0285 44                  LD B,H
0389   0286 50                  LD D,B
0390   0287 00                  NOP
0391   0288 21 00 54            LD HL,5400H
0392   028B 45                  LD B,L
0393   028C 53                  LD D,E
0394   028D 54                  LD D,H
0395   028E 49                  LD C,C
0396   028F 4E                  LD C,(HL)
0397   0290 47                  LD B,A
0398   0291 20 4D               JR NZ,L0043
0399   0293 45                  LD B,L
0400   0294 4D                  LD C,L
0401   0295 4F                  LD C,A
0402   0296 52                  LD D,D
0403   0297 59                  LD E,C
0404   0298 20 2E               JR NZ,L0044
0405   029A 2E 2E               LD L,2EH
0406   029C 20 00               JR NZ,L0045
0407   029E 00          L0045:  NOP
0408   029F 00                  NOP
0409   02A0 82                  ADD A,D
0410   02A1 04                  INC B
0411   02A2 8E                  ADC A,(HL)
0412   02A3 05                  DEC B
0413   02A4 DC 04 C9            CALL C,L0046
0414   02A7 00                  NOP
0415   02A8 00                  NOP
0416   02A9 C3 86 04            JP L0047
0417   02AC C3 FE 04            JP L0048
0418   02AF C3 5F 05            JP L0049
0419   02B2 C3 8C 04    L0005:  JP L0050
0420   02B5 F3          L0028:  DI
0421   02B6 ED 5E               IM 2
0422   02B8 21 A0 02            LD HL,02A0H
0423   02BB 7D                  LD A,L
0424   02BC D3 E8               OUT (0E8H),A
0425   02BE D3 C8               OUT (0C8H),A
0426   02C0 7C                  LD A,H
0427   02C1 ED 47               LD I,A
0428   02C3 FB                  EI
0429   02C4 76                  HALT
0430   02C5 3E 08               LD A,08H
0431   02C7 CD EF 02            CALL L0051
0432   02CA CD FD 02            CALL L0052
0433   02CD CD FD 02            CALL L0052
0434   02D0 3E 03               LD A,03H
0435   02D2 CD EF 02            CALL L0051
0436   02D5 3E 0D               LD A,0DH
0437   02D7 E6 0F               AND 0FH
0438   02D9 07                  RLCA
0439   02DA 07                  RLCA
0440   02DB 07                  RLCA
0441   02DC 07                  RLCA
0442   02DD 47                  LD B,A
0443   02DE 3E 0E               LD A,0EH
0444   02E0 E6 0F       L0043:  AND 0FH
0445   02E2 B0                  OR B
0446   02E3 CD EF 02            CALL L0051
0447   02E6 3E 04               LD A,04H
0448   02E8 07                  RLCA
0449   02E9 E6 FE               AND 0FEH
0450   02EB CD EF 02            CALL L0051
0451   02EE C9                  RET
0452   02EF F5          L0051:  PUSH AF
0453   02F0 DB F0       L0053:  IN A,(0F0H)
0454   02F2 E6 C0               AND 0C0H
0455   02F4 FE 80               CP 80H
0456   02F6 C2 F0 02            JP NZ,L0053
0457   02F9 F1                  POP AF
0458   02FA D3 F1               OUT (0F1H),A
0459   02FC C9                  RET
0460   02FD DB F0       L0052:  IN A,(0F0H)
0461   02FF E6 C0               AND 0C0H
0462   0301 FE C0               CP 0C0H
0463   0303 C2 FD 02            JP NZ,L0052
0464   0306 DB F1               IN A,(0F1H)
0465   0308 C9                  RET
0466   0309 3E 07       L0068:  LD A,07H
0467   030B CD EF 02            CALL L0051
0468   030E 3A D0 FF            LD A,(0FFD0H)
0469   0311 CD EF 02            CALL L0051
0470   0314 FB                  EI
0471   0315 76                  HALT
0472   0316 3E 08               LD A,08H
0473   0318 CD EF 02            CALL L0051
0474   031B CD FD 02            CALL L0052
0475   031E CD FD 02            CALL L0052
0476   0321 AF                  XOR A
0477   0322 32 D1 FF            LD (0FFD1H),A
0478   0325 32 D7 FF            LD (0FFD7H),A
0479   0328 C9                  RET
0480   0329 CD A5 03    L0069:  CALL L0054
0481   032C C0                  RET NZ
0482   032D 3E 0A               LD A,0AH
0483   032F 32 D5 FF            LD (0FFD5H),A
0484   0332 3E 05       L0062:  LD A,05H
0485   0334 D3 C0               OUT (0C0H),A
0486   0336 3E CF               LD A,0CFH
0487   0338 D3 C0               OUT (0C0H),A
0488   033A CD 14 04            CALL L0055
0489   033D 21 26 04            LD HL,0426H
0490   0340 ED B3               OTIR
0491   0342 3E 06               LD A,06H
0492   0344 F6 40               OR 40H
0493   0346 CD E2 03            CALL L0056
0494   0349 CD FE 03            CALL L0057
0495   034C FB          L0059:  EI
0496   034D 76                  HALT
0497   034E DA 63 03            JP C,L0058
0498   0351 DB 98               IN A,(98H)
0499   0353 E6 01               AND 01H
0500   0355 C2 4C 03            JP NZ,L0059
0501   0358 21 E1 04            LD HL,04E1H
0502   035B CD FA 01            CALL L0015
0503   035E D3 98               OUT (98H),A
0504   0360 C3 4C 03            JP L0059
0505   0363 3E 03       L0058:  LD A,03H
0506   0365 D3 CA               OUT (0CAH),A
0507   0367 CD FD 02            CALL L0052
0508   036A CD FD 02            CALL L0052
0509   036D F5                  PUSH AF
0510   036E 06 05               LD B,05H
0511   0370 CD FD 02    L0060:  CALL L0052
0512   0373 05                  DEC B
0513   0374 C2 70 03            JP NZ,L0060
0514   0377 F1                  POP AF
0515   0378 FE 80               CP 80H
0516   037A C8                  RET Z
0517   037B 3A D5 FF            LD A,(0FFD5H)
0518   037E B7                  OR A
0519   037F CA A3 03            JP Z,L0061
0520   0382 3D                  DEC A
0521   0383 32 D5 FF            LD (0FFD5H),A
0522   0386 3A D1 FF            LD A,(0FFD1H)
0523   0389 F5                  PUSH AF
0524   038A 3C                  INC A
0525   038B 32 D1 FF            LD (0FFD1H),A
0526   038E 3A D7 FF            LD A,(0FFD7H)
0527   0391 F5                  PUSH AF
0528   0392 CD A5 03            CALL L0054
0529   0395 F1                  POP AF
0530   0396 32 D7 FF            LD (0FFD7H),A
0531   0399 F1                  POP AF
0532   039A 32 D1 FF            LD (0FFD1H),A
0533   039D CD A5 03            CALL L0054
0534   03A0 C3 32 03            JP L0062
0535   03A3 3C          L0061:  INC A
0536   03A4 C9                  RET
0537   03A5 CD B9 04    L0054:  CALL L0063
0538   03A8 3E 0F               LD A,0FH
0539   03AA CD EF 02            CALL L0051
0540   03AD CD D3 03            CALL L0064
0541   03B0 CD EF 02    L0066:  CALL L0051
0542   03B3 3A D1 FF            LD A,(0FFD1H)
0543   03B6 CD EF 02            CALL L0051
0544   03B9 FB                  EI
0545   03BA 76                  HALT
0546   03BB 3E 08       L0067:  LD A,08H
0547   03BD CD EF 02            CALL L0051
0548   03C0 CD FD 02            CALL L0052
0549   03C3 CD FD 02            CALL L0052
0550   03C6 47                  LD B,A
0551   03C7 3A D1 FF            LD A,(0FFD1H)
0552   03CA B8                  CP B
0553   03CB CA D1 03            JP Z,L0065
0554   03CE AF                  XOR A
0555   03CF 3C                  INC A
0556   03D0 C9                  RET
0557   03D1 AF          L0065:  XOR A
0558   03D2 C9                  RET
0559   03D3 3A D7 FF    L0064:  LD A,(0FFD7H)
0560   03D6 07                  RLCA
0561   03D7 07                  RLCA
0562   03D8 E6 04               AND 04H
0563   03DA C5                  PUSH BC
0564   03DB 47                  LD B,A
0565   03DC 3A D0 FF            LD A,(0FFD0H)
0566   03DF B0                  OR B
0567   03E0 C1                  POP BC
0568   03E1 C9                  RET
0569   03E2 CD EF 02    L0056:  CALL L0051
0570   03E5 CD D3 03            CALL L0064
0571   03E8 CD EF 02            CALL L0051
0572   03EB 3A D1 FF            LD A,(0FFD1H)
0573   03EE CD EF 02            CALL L0051
0574   03F1 3A D7 FF            LD A,(0FFD7H)
0575   03F4 CD EF 02            CALL L0051
0576   03F7 3A D4 FF            LD A,(0FFD4H)
0577   03FA CD EF 02            CALL L0051
0578   03FD C9                  RET
0579   03FE 3E 01       L0057:  LD A,01H
0580   0400 CD EF 02            CALL L0051
0581   0403 3A D4 FF            LD A,(0FFD4H)
0582   0406 CD EF 02            CALL L0051
0583   0409 3E 0A               LD A,0AH
0584   040B CD EF 02            CALL L0051
0585   040E 3E FF               LD A,0FFH
0586   0410 CD EF 02            CALL L0051
0587   0413 C9                  RET
0588   0414 3E 79       L0055:  LD A,79H
0589   0416 D3 C0               OUT (0C0H),A
0590   0418 2A D2 FF            LD HL,(0FFD2H)
0591   041B 7D                  LD A,L
0592   041C D3 C0               OUT (0C0H),A
0593   041E 7C                  LD A,H
0594   041F D3 C0               OUT (0C0H),A
0595   0421 06 0B               LD B,0BH
0596   0423 0E C0               LD C,0C0H
0597   0425 C9                  RET
0598   0426 FF                  RST 38H
0599   0427 00                  NOP
0600   0428 14                  INC D
0601   0429 28 85               JR Z,L0066
0602   042B F1                  POP AF
0603   042C 8A                  ADC A,D
0604   042D CF                  RST 08H
0605   042E 01 CF 87            LD BC,87CFH
0606   0431 FF                  RST 38H
0607   0432 00                  NOP
0608   0433 14                  INC D
0609   0434 28 85               JR Z,L0067
0610   0436 F1                  POP AF
0611   0437 8A                  ADC A,D
0612   0438 CF                  RST 08H
0613   0439 05                  DEC B
0614   043A CF                  RST 08H
0615   043B 87                  ADD A,A
0616   043C 3E 13       L0073:  LD A,13H
0617   043E 32 D8 FF            LD (0FFD8H),A
0618   0441 AF                  XOR A
0619   0442 32 D0 FF            LD (0FFD0H),A
0620   0445 CD 09 03            CALL L0068
0621   0448 21 00 E0            LD HL,0E000H
0622   044B 22 D2 FF            LD (0FFD2H),HL
0623   044E AF          L0072:  XOR A
0624   044F 3C                  INC A
0625   0450 32 D4 FF            LD (0FFD4H),A
0626   0453 CD 29 03    L0071:  CALL L0069
0627   0456 C2 89 06            JP NZ,L0070
0628   0459 11 00 01            LD DE,0100H
0629   045C 2A D2 FF            LD HL,(0FFD2H)
0630   045F 19                  ADD HL,DE
0631   0460 22 D2 FF            LD (0FFD2H),HL
0632   0463 3A D4 FF            LD A,(0FFD4H)
0633   0466 3C                  INC A
0634   0467 32 D4 FF            LD (0FFD4H),A
0635   046A 21 D8 FF            LD HL,0FFD8H
0636   046D BE                  CP (HL)
0637   046E C2 53 04            JP NZ,L0071
0638   0471 3A D7 FF            LD A,(0FFD7H)
0639   0474 B7                  OR A
0640   0475 C0                  RET NZ
0641   0476 3C                  INC A
0642   0477 32 D7 FF            LD (0FFD7H),A
0643   047A 3E 0E               LD A,0EH
0644   047C 32 D8 FF            LD (0FFD8H),A
0645   047F C3 4E 04            JP L0072
0646   0482 FB                  EI
0647   0483 37                  SCF
0648   0484 ED 4D               RETI
0649   0486 CD 3C 04    L0047:  CALL L0073
0650   0489 C3 03 00            JP L0006
0651   048C CD 3C 04    L0050:  CALL L0073
0652   048F 3A 00 E0            LD A,(0E000H)
0653   0492 FE C3               CP 0C3H
0654   0494 CA A2 04            JP Z,L0074
0655   0497 FE 31               CP 31H
0656   0499 CA A2 04            JP Z,L0074
0657   049C 21 4B 05            LD HL,054BH
0658   049F C3 92 06            JP L0075
0659   04A2 C3 00 F6    L0074:  JP L0076
0660   04A5 DB 98       L0079:  IN A,(98H)
0661   04A7 E6 01               AND 01H
0662   04A9 C9                  RET
0663   04AA 3E FF       L0081:  LD A,0FFH
0664   04AC C5                  PUSH BC
0665   04AD 06 FF       L0078:  LD B,0FFH
0666   04AF 05          L0077:  DEC B
0667   04B0 C2 AF 04            JP NZ,L0077
0668   04B3 3D                  DEC A
0669   04B4 C2 AD 04            JP NZ,L0078
0670   04B7 C1                  POP BC
0671   04B8 C9                  RET
0672   04B9 CD A5 04    L0063:  CALL L0079
0673   04BC C2 C4 04            JP NZ,L0080
0674   04BF D3 98               OUT (98H),A
0675   04C1 CD AA 04            CALL L0081
0676   04C4 AF          L0080:  XOR A
0677   04C5 D3 98               OUT (98H),A
0678   04C7 3E 47               LD A,47H
0679   04C9 D3 C8               OUT (0C8H),A
0680   04CB D3 C9               OUT (0C9H),A
0681   04CD 3E 82               LD A,82H
0682   04CF D3 C8               OUT (0C8H),A
0683   04D1 D3 C9               OUT (0C9H),A
0684   04D3 3E A7               LD A,0A7H
0685   04D5 D3 CA               OUT (0CAH),A
0686   04D7 3E FF               LD A,0FFH
0687   04D9 D3 CA               OUT (0CAH),A
0688   04DB C9                  RET
0689   04DC FB                  EI
0690   04DD 37                  SCF
0691   04DE 3F                  CCF
0692   04DF ED 4D               RETI
0693   04E1 21 00 46            LD HL,4600H
0694   04E4 4C                  LD C,H
0695   04E5 4F                  LD C,A
0696   04E6 50                  LD D,B
0697   04E7 50                  LD D,B
0698   04E8 59                  LD E,C
0699   04E9 20 44               JR NZ,L0082
0700   04EB 49                  LD C,C
0701   04EC 53                  LD D,E
0702   04ED 4B                  LD C,E
0703   04EE 20 4E               JR NZ,L0083
0704   04F0 4F                  LD C,A
0705   04F1 54                  LD D,H
0706   04F2 20 52               JR NZ,L0084
0707   04F4 45                  LD B,L
0708   04F5 41                  LD B,C
0709   04F6 44                  LD B,H
0710   04F7 59                  LD E,C
0711   04F8 20 21               JR NZ,L0085
0712   04FA 21 21 21            LD HL,2121H
0713   04FD 00                  NOP
0714   04FE CD C3 05    L0048:  CALL L0086
0715   0501 3A 00 E0            LD A,(0E000H)
0716   0504 FE 31               CP 31H
0717   0506 CA 0F 05            JP Z,L0087
0718   0509 21 4B 05            LD HL,054BH
0719   050C C3 92 06            JP L0075
0720   050F 21 00 00    L0087:  LD HL,0000H
0721   0512 11 00 20            LD DE,2000H
0722   0515 01 00 08            LD BC,0800H
0723   0518 ED B0               LDIR
0724   051A 21 1C 07            LD HL,071CH
0725   051D 11 00 C0            LD DE,0C000H
0726   0520 01 6C 00            LD BC,006CH
0727   0523 ED B0               LDIR
0728   0525 F3                  DI
0729   0526 3E 03               LD A,03H
0730   0528 D3 C8               OUT (0C8H),A
0731   052A D3 C9               OUT (0C9H),A
0732   052C 21 00 C0            LD HL,0C000H
0733   052F 7C          L0082:  LD A,H
0734   0530 ED 47               LD I,A
0735   0532 7D                  LD A,L
0736   0533 D3 E8               OUT (0E8H),A
0737   0535 D3 C8               OUT (0C8H),A
0738   0537 3E 47               LD A,47H
0739   0539 D3 C8               OUT (0C8H),A
0740   053B 3E FF               LD A,0FFH
0741   053D D3 C8               OUT (0C8H),A
0742   053F 3E C7               LD A,0C7H
0743   0541 D3 C9               OUT (0C9H),A
0744   0543 3E 64               LD A,64H
0745   0545 D3 C9               OUT (0C9H),A
0746   0547 FB                  EI
0747   0548 C3 00 F6            JP L0076
0748   054B 21 00 4E            LD HL,4E00H
0749   054E 4F                  LD C,A
0750   054F 20 53               JR NZ,L0088
0751   0551 59                  LD E,C
0752   0552 53                  LD D,E
0753   0553 54                  LD D,H
0754   0554 45                  LD B,L
0755   0555 4D                  LD C,L
0756   0556 20 4F               JR NZ,L0089
0757   0558 4E                  LD C,(HL)
0758   0559 20 44               JR NZ,L0090
0759   055B 49                  LD C,C
0760   055C 53                  LD D,E
0761   055D 4B                  LD C,E
0762   055E 00                  NOP
0763   055F CD C3 05    L0049:  CALL L0086
0764   0562 C3 03 00            JP L0006
0765   0565 AF          L0101:  XOR A
0766   0566 D3 12               OUT (12H),A
0767   0568 F3                  DI
0768   0569 3E 47               LD A,47H
0769   056B D3 C8               OUT (0C8H),A
0770   056D 3E FF               LD A,0FFH
0771   056F D3 C8               OUT (0C8H),A
0772   0571 3E C7               LD A,0C7H
0773   0573 D3 C9               OUT (0C9H),A
0774   0575 3E 50               LD A,50H
0775   0577 D3 C9               OUT (0C9H),A
0776   0579 FB                  EI
0777   057A CD B4 05            CALL L0091
0778   057D 21 10 07    L0094:  LD HL,0710H
0779   0580 CD F5 05            CALL L0092
0780   0583 CD 1A 06            CALL L0093
0781   0586 C2 7D 05            JP NZ,L0094
0782   0589 3E 03               LD A,03H
0783   058B D3 C9               OUT (0C9H),A
0784   058D C9                  RET
0785   058E 3E 03               LD A,03H
0786   0590 D3 C9               OUT (0C9H),A
0787   0592 CD 9C 05            CALL L0095
0788   0595 FB                  EI
0789   0596 21 9E 05            LD HL,059EH
0790   0599 C3 92 06            JP L0075
0791   059C ED 4D       L0095:  RETI
0792   059E 21 00 48            LD HL,4800H
0793   05A1 41                  LD B,C
0794   05A2 52                  LD D,D
0795   05A3 44                  LD B,H
0796   05A4 20 44       L0088:  JR NZ,L0096
0797   05A6 49                  LD C,C
0798   05A7 53          L0089:  LD D,E
0799   05A8 4B                  LD C,E
0800   05A9 20 4E               JR NZ,L0097
0801   05AB 4F                  LD C,A
0802   05AC 54                  LD D,H
0803   05AD 20 52               JR NZ,L0098
0804   05AF 45                  LD B,L
0805   05B0 41                  LD B,C
0806   05B1 44                  LD B,H
0807   05B2 59                  LD E,C
0808   05B3 00                  NOP
0809   05B4 21 02 07    L0091:  LD HL,0702H
0810   05B7 CD 0A 06            CALL L0099
0811   05BA CD 1A 06            CALL L0093
0812   05BD C8                  RET Z
0813   05BE 3E 34               LD A,34H
0814   05C0 C3 8F 06            JP L0100
0815   05C3 CD 65 05    L0086:  CALL L0101
0816   05C6 3E C3               LD A,0C3H
0817   05C8 D3 C0               OUT (0C0H),A
0818   05CA 21 16 07            LD HL,0716H
0819   05CD CD F5 05            CALL L0092
0820   05D0 DB 10       L0102:  IN A,(10H)
0821   05D2 E6 40               AND 40H
0822   05D4 CA D0 05            JP Z,L0102
0823   05D7 DB 10               IN A,(10H)
0824   05D9 E6 10               AND 10H
0825   05DB C2 EC 05            JP NZ,L0103
0826   05DE 3E 22               LD A,22H
0827   05E0 D3 10               OUT (10H),A
0828   05E2 CD 61 06            CALL L0104
0829   05E5 DB 10       L0105:  IN A,(10H)
0830   05E7 E6 10               AND 10H
0831   05E9 CA E5 05            JP Z,L0105
0832   05EC CD 1A 06            CALL L0093
0833   05EF C8                  RET Z
0834   05F0 3E 32               LD A,32H
0835   05F2 C3 8F 06            JP L0100
0836   05F5 CD 37 06    L0092:  CALL L0106
0837   05F8 CD 53 06    L0108:  CALL L0107
0838   05FB 47                  LD B,A
0839   05FC E6 10               AND 10H
0840   05FE C8                  RET Z
0841   05FF 78                  LD A,B
0842   0600 E6 40               AND 40H
0843   0602 C0                  RET NZ
0844   0603 7E                  LD A,(HL)
0845   0604 D3 11               OUT (11H),A
0846   0606 23                  INC HL
0847   0607 C3 F8 05            JP L0108
0848   060A CD 37 06            CALL L0106
0849   060D CD 53 06            CALL L0107
0850   0610 E6 40               AND 40H
0851   0612 C0                  RET NZ
0852   0613 7E                  LD A,(HL)
0853   0614 D3 11               OUT (11H),A
0854   0616 23                  INC HL
0855   0617 C3 0D 06    L0115:  JP L0109
0856   061A CD 53 06    L0093:  CALL L0107
0857   061D E6 10               AND 10H
0858   061F C2 27 06            JP NZ,L0110
0859   0622 3E 42               LD A,42H
0860   0624 C3 8F 06            JP L0100
0861   0627 DB 11       L0110:  IN A,(11H)
0862   0629 47                  LD B,A
0863   062A 23                  INC HL
0864   062B CD 53 06            CALL L0107
0865   062E DB 11               IN A,(11H)
0866   0630 AF                  XOR A
0867   0631 D3 10               OUT (10H),A
0868   0633 78                  LD A,B
0869   0634 E6 03               AND 03H
0870   0636 C9                  RET
0871   0637 DB 10       L0106:  IN A,(10H)
0872   0639 E6 08               AND 08H
0873   063B CA 43 06            JP Z,L0111
0874   063E 3E 41               LD A,41H
0875   0640 C3 8F 06            JP L0100
0876   0643 3E 01       L0111:  LD A,01H
0877   0645 D3 10               OUT (10H),A
0878   0647 DB 10       L0112:  IN A,(10H)
0879   0649 E6 08               AND 08H
0880   064B CA 47 06            JP Z,L0112
0881   064E 3E 02               LD A,02H
0882   0650 D3 10               OUT (10H),A
0883   0652 C9                  RET
0884   0653 DB 10       L0107:  IN A,(10H)
0885   0655 17                  RLA
0886   0656 D2 53 06            JP NC,L0107
0887   0659 1F                  RRA
0888   065A C9                  RET
0889   065B 21 7A 06            LD HL,067AH
0890   065E C3 64 06            JP L0113
0891   0661 21 6B 06    L0104:  LD HL,066BH
0892   0664 0E C0       L0113:  LD C,0C0H
0893   0666 06 0F               LD B,0FH
0894   0668 ED B3               OTIR
0895   066A C9                  RET
0896   066B 79                  LD A,C
0897   066C 00                  NOP
0898   066D E0                  RET PO
0899   066E FF                  RST 38H
0900   066F 1E 14               LD E,14H
0901   0671 28 95               JR Z,L0114
0902   0673 11 00 8A            LD DE,8A00H
0903   0676 CF                  RST 08H
0904   0677 01 CF 87            LD BC,87CFH
0905   067A 79                  LD A,C
0906   067B 00                  NOP
0907   067C E0                  RET PO
0908   067D FF                  RST 38H
0909   067E 1E 14               LD E,14H
0910   0680 28 95               JR Z,L0115
0911   0682 11 00 8A            LD DE,8A00H
0912   0685 CF                  RST 08H
0913   0686 05                  DEC B
0914   0687 CF                  RST 08H
0915   0688 87                  ADD A,A
0916   0689 21 98 06    L0070:  LD HL,0698H
0917   068C C3 92 06            JP L0075
0918   068F 21 CF 06    L0100:  LD HL,06CFH
0919   0692 CD FA 01    L0075:  CALL L0015
0920   0695 C3 03 00            JP L0006
0921   0698 21 00 46            LD HL,4600H
0922   069B 4C                  LD C,H
0923   069C 4F                  LD C,A
0924   069D 50                  LD D,B
0925   069E 50                  LD D,B
0926   069F 59                  LD E,C
0927   06A0 20 44               JR NZ,L0116
0928   06A2 49                  LD C,C
0929   06A3 53                  LD D,E
0930   06A4 4B                  LD C,E
0931   06A5 20 4D               JR NZ,L0117
0932   06A7 41                  LD B,C
0933   06A8 4C                  LD C,H
0934   06A9 46                  LD B,(HL)
0935   06AA 55                  LD D,L
0936   06AB 4E                  LD C,(HL)
0937   06AC 43                  LD B,E
0938   06AD 54                  LD D,H
0939   06AE 49                  LD C,C
0940   06AF 4F                  LD C,A
0941   06B0 4E                  LD C,(HL)
0942   06B1 20 21               JR NZ,L0118
0943   06B3 21 21 52            LD HL,5221H
0944   06B6 65                  LD H,L
0945   06B7 74                  LD (HL),H
0946   06B8 72                  LD (HL),D
0947   06B9 79                  LD A,C
0948   06BA 20 77               JR NZ,L0119
0949   06BC 69                  LD L,C
0950   06BD 74                  LD (HL),H
0951   06BE 68                  LD L,B
0952   06BF 20 63               JR NZ,L0120
0953   06C1 6F                  LD L,A
0954   06C2 6D                  LD L,L
0955   06C3 6D                  LD L,L
0956   06C4 61                  LD H,C
0957   06C5 6E                  LD L,(HL)
0958   06C6 64                  LD H,H
0959   06C7 20 52               JR NZ,L0121
0960   06C9 20 6F               JR NZ,L0122
0961   06CB 72                  LD (HL),D
0962   06CC 20 46               JR NZ,L0123
0963   06CE 00                  NOP
0964   06CF 21 00 48            LD HL,4800H
0965   06D2 41                  LD B,C
0966   06D3 52                  LD D,D
0967   06D4 44          L0118:  LD B,H
0968   06D5 20 44               JR NZ,L0121
0969   06D7 49                  LD C,C
0970   06D8 53                  LD D,E
0971   06D9 4B                  LD C,E
0972   06DA 20 4D               JR NZ,L0124
0973   06DC 41                  LD B,C
0974   06DD 4C                  LD C,H
0975   06DE 46                  LD B,(HL)
0976   06DF 55                  LD D,L
0977   06E0 4E                  LD C,(HL)
0978   06E1 43                  LD B,E
0979   06E2 54                  LD D,H
0980   06E3 49                  LD C,C
0981   06E4 4F                  LD C,A
0982   06E5 4E                  LD C,(HL)
0983   06E6 20 3E       L0116:  JR NZ,L0125
0984   06E8 3E 3E               LD A,3EH
0985   06EA 20 52               JR NZ,L0126
0986   06EC 45                  LD B,L
0987   06ED 54                  LD D,H
0988   06EE 52                  LD D,D
0989   06EF 59                  LD E,C
0990   06F0 20 57               JR NZ,L0127
0991   06F2 49                  LD C,C
0992   06F3 54                  LD D,H
0993   06F4 48          L0117:  LD C,B
0994   06F5 20 43               JR NZ,L0122
0995   06F7 4F                  LD C,A
0996   06F8 4D                  LD C,L
0997   06F9 4D                  LD C,L
0998   06FA 41                  LD B,C
0999   06FB 4E                  LD C,(HL)
1000   06FC 44                  LD B,H
1001   06FD 20 20               JR NZ,L0128
1002   06FF 41                  LD B,C
1003   0700 20 00               JR NZ,L0129
1004   0702 0C          L0129:  INC C
1005   0703 00                  NOP
1006   0704 00                  NOP
1007   0705 00                  NOP
1008   0706 00                  NOP
1009   0707 00                  NOP
1010   0708 01 32 04            LD BC,0432H
1011   070B 00                  NOP
1012   070C 80                  ADD A,B
1013   070D 00                  NOP
1014   070E 40                  LD B,B
1015   070F 0B                  DEC BC
1016   0710 01 00 00            LD BC,0000H
1017   0713 00                  NOP
1018   0714 00          L0123:  NOP
1019   0715 00                  NOP
1020   0716 08                  EX AF,AF'
1021   0717 00                  NOP
1022   0718 00                  NOP
1023   0719 00                  NOP
1024   071A 1F                  RRA
1025   071B 00          L0121:  NOP
1026   071C 04                  INC B
1027   071D C0                  RET NZ
1028   071E 04                  INC B
1029   071F C0          L0128:  RET NZ
1030   0720 F3                  DI
1031   0721 3E 03               LD A,03H
1032   0723 D3 C8               OUT (0C8H),A
1033   0725 D3 C9               OUT (0C9H),A
1034   0727 D3 88               OUT (88H),A
1035   0729 21 00 20    L0124:  LD HL,2000H
1036   072C 11 00 00            LD DE,0000H
1037   072F 01 00 08            LD BC,0800H
1038   0732 ED B0               LDIR
1039   0734 21 A0 02            LD HL,02A0H
1040   0737 7D                  LD A,L
1041   0738 D3 E8               OUT (0E8H),A
1042   073A D3 C8       L0122:  OUT (0C8H),A
1043   073C 7C                  LD A,H
1044   073D ED 47               LD I,A
1045   073F FB                  EI
1046   0740 CD 9C 05            CALL L0095
1047   0743 21 49 07            LD HL,0749H
1048   0746 C3 FA 01            JP L0015
1049   0749 11 00 4C    L0127:  LD DE,4C00H
1050   074C 4F                  LD C,A
1051   074D 41                  LD B,C
1052   074E 44                  LD B,H
1053   074F 49                  LD C,C
1054   0750 4E                  LD C,(HL)
1055   0751 47                  LD B,A
1056   0752 20 45               JR NZ,L0130
1057   0754 52                  LD D,D
1058   0755 52                  LD D,D
1059   0756 4F                  LD C,A
1060   0757 52                  LD D,D
1061   0758 20 46               JR NZ,L0131
1062   075A 52                  LD D,D
1063   075B 4F                  LD C,A
1064   075C 4D                  LD C,L
1065   075D 20 48               JR NZ,L0132
1066   075F 41                  LD B,C
1067   0760 52                  LD D,D
1068   0761 44                  LD B,H
1069   0762 20 44               JR NZ,L0133
1070   0764 49                  LD C,C
1071   0765 53                  LD D,E
1072   0766 4B                  LD C,E
1073   0767 20 54               JR NZ,L0134
1074   0769 52                  LD D,D
1075   076A 59                  LD E,C
1076   076B 20 54               JR NZ,L0135
1077   076D 4F                  LD C,A
1078   076E 20 4C               JR NZ,L0136
1079   0770 4F                  LD C,A
1080   0771 41                  LD B,C
1081   0772 44                  LD B,H
1082   0773 20 53               JR NZ,L0137
1083   0775 59                  LD E,C
1084   0776 53                  LD D,E
1085   0777 54                  LD D,H
1086   0778 45                  LD B,L
1087   0779 4D                  LD C,L
1088   077A 20 46               JR NZ,L0138
1089   077C 52                  LD D,D
1090   077D 4F                  LD C,A
1091   077E 4D                  LD C,L
1092   077F 20 46               JR NZ,L0139
1093   0781 4C                  LD C,H
1094   0782 4F                  LD C,A
1095   0783 50                  LD D,B
1096   0784 50                  LD D,B
1097   0785 59                  LD E,C
1098   0786 20 00               JR NZ,L0140
1099   0788 FF          L0140:  RST 38H
1100   0789 1A                  LD A,(DE)
1101   078A 1A                  LD A,(DE)
1102   078B 1A                  LD A,(DE)
1103   078C 1A                  LD A,(DE)
1104   078D 1A                  LD A,(DE)
1105   078E 1A                  LD A,(DE)
1106   078F 1A                  LD A,(DE)
1107   0790 1A                  LD A,(DE)
1108   0791 1A                  LD A,(DE)
1109   0792 1A                  LD A,(DE)
1110   0793 1A                  LD A,(DE)
1111   0794 1A                  LD A,(DE)
1112   0795 1A                  LD A,(DE)
1113   0796 1A                  LD A,(DE)
1114   0797 1A                  LD A,(DE)
1115   0798 1A                  LD A,(DE)
1116   0799 1A          L0130:  LD A,(DE)
1117   079A 1A                  LD A,(DE)
1118   079B 1A                  LD A,(DE)
1119   079C 1A                  LD A,(DE)
1120   079D 1A                  LD A,(DE)
1121   079E 1A                  LD A,(DE)
1122   079F 1A                  LD A,(DE)
1123   07A0 1A          L0131:  LD A,(DE)
1124   07A1 1A                  LD A,(DE)
1125   07A2 1A                  LD A,(DE)
1126   07A3 1A                  LD A,(DE)
1127   07A4 1A                  LD A,(DE)
1128   07A5 1A                  LD A,(DE)
1129   07A6 1A                  LD A,(DE)
1130   07A7 1A          L0132:  LD A,(DE)
1131   07A8 1A          L0133:  LD A,(DE)
1132   07A9 1A                  LD A,(DE)
1133   07AA 1A                  LD A,(DE)
1134   07AB 1A                  LD A,(DE)
1135   07AC 1A                  LD A,(DE)
1136   07AD 1A                  LD A,(DE)
1137   07AE 1A                  LD A,(DE)
1138   07AF 1A                  LD A,(DE)
1139   07B0 1A                  LD A,(DE)
1140   07B1 1A                  LD A,(DE)
1141   07B2 1A                  LD A,(DE)
1142   07B3 1A                  LD A,(DE)
1143   07B4 1A                  LD A,(DE)
1144   07B5 1A                  LD A,(DE)
1145   07B6 1A                  LD A,(DE)
1146   07B7 1A                  LD A,(DE)
1147   07B8 1A                  LD A,(DE)
1148   07B9 1A                  LD A,(DE)
1149   07BA 1A                  LD A,(DE)
1150   07BB 1A                  LD A,(DE)
1151   07BC 1A          L0136:  LD A,(DE)
1152   07BD 1A          L0134:  LD A,(DE)
1153   07BE 1A                  LD A,(DE)
1154   07BF 1A                  LD A,(DE)
1155   07C0 1A                  LD A,(DE)
1156   07C1 1A          L0135:  LD A,(DE)
1157   07C2 1A          L0138:  LD A,(DE)
1158   07C3 1A                  LD A,(DE)
1159   07C4 1A                  LD A,(DE)
1160   07C5 1A                  LD A,(DE)
1161   07C6 1A                  LD A,(DE)
1162   07C7 1A          L0139:  LD A,(DE)
1163   07C8 1A          L0137:  LD A,(DE)
1164   07C9 1A                  LD A,(DE)
1165   07CA 1A                  LD A,(DE)
1166   07CB 1A                  LD A,(DE)
1167   07CC 1A                  LD A,(DE)
1168   07CD 1A                  LD A,(DE)
1169   07CE 1A                  LD A,(DE)
1170   07CF 1A                  LD A,(DE)
1171   07D0 1A                  LD A,(DE)
1172   07D1 1A                  LD A,(DE)
1173   07D2 1A                  LD A,(DE)
1174   07D3 1A                  LD A,(DE)
1175   07D4 1A                  LD A,(DE)
1176   07D5 1A                  LD A,(DE)
1177   07D6 1A                  LD A,(DE)
1178   07D7 1A                  LD A,(DE)
1179   07D8 1A                  LD A,(DE)
1180   07D9 1A                  LD A,(DE)
1181   07DA 1A                  LD A,(DE)
1182   07DB 1A                  LD A,(DE)
1183   07DC 1A                  LD A,(DE)
1184   07DD 1A                  LD A,(DE)
1185   07DE 1A                  LD A,(DE)
1186   07DF 1A                  LD A,(DE)
1187   07E0 1A                  LD A,(DE)
1188   07E1 1A                  LD A,(DE)
1189   07E2 1A                  LD A,(DE)
1190   07E3 1A                  LD A,(DE)
1191   07E4 1A                  LD A,(DE)
1192   07E5 1A                  LD A,(DE)
1193   07E6 1A                  LD A,(DE)
1194   07E7 1A                  LD A,(DE)
1195   07E8 1A                  LD A,(DE)
1196   07E9 1A                  LD A,(DE)
1197   07EA 1A                  LD A,(DE)
1198   07EB 1A                  LD A,(DE)
1199   07EC 1A                  LD A,(DE)
1200   07ED 1A                  LD A,(DE)
1201   07EE 1A                  LD A,(DE)
1202   07EF 1A                  LD A,(DE)
1203   07F0 1A                  LD A,(DE)
1204   07F1 1A                  LD A,(DE)
1205   07F2 1A                  LD A,(DE)
1206   07F3 1A                  LD A,(DE)
1207   07F4 1A                  LD A,(DE)
1208   07F5 1A                  LD A,(DE)
1209   07F6 1A                  LD A,(DE)
1210   07F7 1A                  LD A,(DE)
1211   07F8 1A                  LD A,(DE)
1212   07F9 1A                  LD A,(DE)
1213   07FA 1A                  LD A,(DE)
1214   07FB 1A                  LD A,(DE)
1215   07FC 1A                  LD A,(DE)
1216   07FD 1A                  LD A,(DE)
1217   07FE 1A                  LD A,(DE)
1218   07FF 1A                  LD A,(DE)
1219   0800 7E                  LD A,(HL)
1220   0801 7E                  LD A,(HL)
1221   0802 7E                  LD A,(HL)
1222   0803 7E                  LD A,(HL)
1223   0804 7E                  LD A,(HL)
1224   0805 7E                  LD A,(HL)
1225   0806 7E                  LD A,(HL)
1226   0807 7E                  LD A,(HL)
1227   0808 7E                  LD A,(HL)
1228   0809 7E                  LD A,(HL)
1229   080A 7E                  LD A,(HL)
1230   080B 7E                  LD A,(HL)
1231   080C 7E                  LD A,(HL)
1232   080D 7E                  LD A,(HL)
1233   080E 7E                  LD A,(HL)
1234   080F 7E                  LD A,(HL)
1235   0810 7E                  LD A,(HL)
1236   0811 7E                  LD A,(HL)
1237   0812 7E                  LD A,(HL)
1238   0813 7E                  LD A,(HL)
1239   0814 7E                  LD A,(HL)
1240   0815 7E                  LD A,(HL)
1241   0816 7E                  LD A,(HL)
1242   0817 7E                  LD A,(HL)
1243   0818 7E                  LD A,(HL)
1244   0819 7E                  LD A,(HL)
1245   081A 7E                  LD A,(HL)
1246   081B 7E                  LD A,(HL)
1247   081C 7E                  LD A,(HL)
1248   081D 7E                  LD A,(HL)
1249   081E 7E                  LD A,(HL)
1250   081F 7E                  LD A,(HL)
1251   0820 7E                  LD A,(HL)
1252   0821 7E                  LD A,(HL)
1253   0822 7E                  LD A,(HL)
1254   0823 7E                  LD A,(HL)
1255   0824 7E                  LD A,(HL)
1256   0825 7E                  LD A,(HL)
1257   0826 7E                  LD A,(HL)
1258   0827 7E                  LD A,(HL)
1259   0828 7E                  LD A,(HL)
1260   0829 7E                  LD A,(HL)
1261   082A 7E                  LD A,(HL)
1262   082B 7E                  LD A,(HL)
1263   082C 7E                  LD A,(HL)
1264   082D 7E                  LD A,(HL)
1265   082E 7E                  LD A,(HL)
1266   082F 7E                  LD A,(HL)
1267   0830 7E                  LD A,(HL)
1268   0831 7E                  LD A,(HL)
1269   0832 7E                  LD A,(HL)
1270   0833 7E                  LD A,(HL)
1271   0834 7E                  LD A,(HL)
1272   0835 7E                  LD A,(HL)
1273   0836 7E                  LD A,(HL)
1274   0837 7E                  LD A,(HL)
1275   0838 7E                  LD A,(HL)
1276   0839 7E                  LD A,(HL)
1277   083A 7E                  LD A,(HL)
1278   083B 7E                  LD A,(HL)
1279   083C 7E                  LD A,(HL)
1280   083D 7E                  LD A,(HL)
1281   083E 7E                  LD A,(HL)
1282   083F 7E                  LD A,(HL)
1283   0840 7E                  LD A,(HL)
1284   0841 7E                  LD A,(HL)
1285   0842 7E                  LD A,(HL)
1286   0843 7E                  LD A,(HL)
1287   0844 7E                  LD A,(HL)
1288   0845 7E                  LD A,(HL)
1289   0846 7E                  LD A,(HL)
1290   0847 7E                  LD A,(HL)
1291   0848 7E                  LD A,(HL)
1292   0849 7E                  LD A,(HL)
1293   084A 7E                  LD A,(HL)
1294   084B 7E                  LD A,(HL)
1295   084C 7E                  LD A,(HL)
1296   084D 7E                  LD A,(HL)
1297   084E 7E                  LD A,(HL)
1298   084F 7E                  LD A,(HL)
1299   0850 7E                  LD A,(HL)
1300   0851 7E                  LD A,(HL)
1301   0852 7E                  LD A,(HL)
1302   0853 7E                  LD A,(HL)
1303   0854 7E                  LD A,(HL)
1304   0855 7E                  LD A,(HL)
1305   0856 7E                  LD A,(HL)
1306   0857 7E                  LD A,(HL)
1307   0858 7E                  LD A,(HL)
1308   0859 7E                  LD A,(HL)
1309   085A 7E                  LD A,(HL)
1310   085B 7E                  LD A,(HL)
1311   085C 7E                  LD A,(HL)
1312   085D 7E                  LD A,(HL)
1313   085E 7E                  LD A,(HL)
1314   085F 7E                  LD A,(HL)
1315   0860 7E                  LD A,(HL)
1316   0861 7E                  LD A,(HL)
1317   0862 7E                  LD A,(HL)
1318   0863 7E                  LD A,(HL)
1319   0864 7E                  LD A,(HL)
1320   0865 7E                  LD A,(HL)
1321   0866 7E                  LD A,(HL)
1322   0867 7E                  LD A,(HL)
1323   0868 7E                  LD A,(HL)
1324   0869 7E                  LD A,(HL)
1325   086A 7E                  LD A,(HL)
1326   086B 7E                  LD A,(HL)
1327   086C 7E                  LD A,(HL)
1328   086D 7E                  LD A,(HL)
1329   086E 7E                  LD A,(HL)
1330   086F 7E                  LD A,(HL)
1331   0870 7E                  LD A,(HL)
1332   0871 7E                  LD A,(HL)
1333   0872 7E                  LD A,(HL)
1334   0873 7E                  LD A,(HL)
1335   0874 7E                  LD A,(HL)
1336   0875 7E                  LD A,(HL)
1337   0876 7E                  LD A,(HL)
1338   0877 7E                  LD A,(HL)
1339   0878 7E                  LD A,(HL)
1340   0879 7E                  LD A,(HL)
1341   087A 7E                  LD A,(HL)
1342   087B 7E                  LD A,(HL)
1343   087C 7E                  LD A,(HL)
1344   087D 7E                  LD A,(HL)
1345   087E 7E                  LD A,(HL)
1346   087F 7E                  LD A,(HL)
1347   0880 7E                  LD A,(HL)
1348   0881 7E                  LD A,(HL)
1349   0882 7E                  LD A,(HL)
1350   0883 7E                  LD A,(HL)
1351   0884 7E                  LD A,(HL)
1352   0885 7E                  LD A,(HL)
1353   0886 7E                  LD A,(HL)
1354   0887 7E                  LD A,(HL)
1355   0888 7E                  LD A,(HL)
1356   0889 7E                  LD A,(HL)
1357   088A 7E                  LD A,(HL)
1358   088B 7E                  LD A,(HL)
1359   088C 7E                  LD A,(HL)
1360   088D 7E                  LD A,(HL)
1361   088E 7E                  LD A,(HL)
1362   088F 7E                  LD A,(HL)
1363   0890 7E                  LD A,(HL)
1364   0891 7E                  LD A,(HL)
1365   0892 7E                  LD A,(HL)
1366   0893 7E                  LD A,(HL)
1367   0894 7E                  LD A,(HL)
1368   0895 7E                  LD A,(HL)
1369   0896 7E                  LD A,(HL)
1370   0897 7E                  LD A,(HL)
1371   0898 7E                  LD A,(HL)
1372   0899 7E                  LD A,(HL)
1373   089A 7E                  LD A,(HL)
1374   089B 7E                  LD A,(HL)
1375   089C 7E                  LD A,(HL)
1376   089D 7E                  LD A,(HL)
1377   089E 7E                  LD A,(HL)
1378   089F 7E                  LD A,(HL)
1379   08A0 7E                  LD A,(HL)
1380   08A1 7E                  LD A,(HL)
1381   08A2 7E                  LD A,(HL)
1382   08A3 7E                  LD A,(HL)
1383   08A4 7E                  LD A,(HL)
1384   08A5 7E                  LD A,(HL)
1385   08A6 7E                  LD A,(HL)
1386   08A7 7E                  LD A,(HL)
1387   08A8 7E                  LD A,(HL)
1388   08A9 7E                  LD A,(HL)
1389   08AA 7E                  LD A,(HL)
1390   08AB 7E                  LD A,(HL)
1391   08AC 7E                  LD A,(HL)
1392   08AD 7E                  LD A,(HL)
1393   08AE 7E                  LD A,(HL)
1394   08AF 7E                  LD A,(HL)
1395   08B0 7E                  LD A,(HL)
1396   08B1 7E                  LD A,(HL)
1397   08B2 7E                  LD A,(HL)
1398   08B3 7E                  LD A,(HL)
1399   08B4 7E                  LD A,(HL)
1400   08B5 7E                  LD A,(HL)
1401   08B6 7E                  LD A,(HL)
1402   08B7 7E                  LD A,(HL)
1403   08B8 7E                  LD A,(HL)
1404   08B9 7E                  LD A,(HL)
1405   08BA 7E                  LD A,(HL)
1406   08BB 7E                  LD A,(HL)
1407   08BC 7E                  LD A,(HL)
1408   08BD 7E                  LD A,(HL)
1409   08BE 7E                  LD A,(HL)
1410   08BF 7E                  LD A,(HL)
1411   08C0 7E                  LD A,(HL)
1412   08C1 7E                  LD A,(HL)
1413   08C2 7E                  LD A,(HL)
1414   08C3 7E                  LD A,(HL)
1415   08C4 7E                  LD A,(HL)
1416   08C5 7E                  LD A,(HL)
1417   08C6 7E                  LD A,(HL)
1418   08C7 7E                  LD A,(HL)
1419   08C8 7E                  LD A,(HL)
1420   08C9 7E                  LD A,(HL)
1421   08CA 7E                  LD A,(HL)
1422   08CB 7E                  LD A,(HL)
1423   08CC 7E                  LD A,(HL)
1424   08CD 7E                  LD A,(HL)
1425   08CE 7E                  LD A,(HL)
1426   08CF 7E                  LD A,(HL)
1427   08D0 7E                  LD A,(HL)
1428   08D1 7E                  LD A,(HL)
1429   08D2 7E                  LD A,(HL)
1430   08D3 7E                  LD A,(HL)
1431   08D4 7E                  LD A,(HL)
1432   08D5 7E                  LD A,(HL)
1433   08D6 7E                  LD A,(HL)
1434   08D7 7E                  LD A,(HL)
1435   08D8 7E                  LD A,(HL)
1436   08D9 7E                  LD A,(HL)
1437   08DA 7E                  LD A,(HL)
1438   08DB 7E                  LD A,(HL)
1439   08DC 7E                  LD A,(HL)
1440   08DD 7E                  LD A,(HL)
1441   08DE 7E                  LD A,(HL)
1442   08DF 7E                  LD A,(HL)
1443   08E0 7E                  LD A,(HL)
1444   08E1 7E                  LD A,(HL)
1445   08E2 7E                  LD A,(HL)
1446   08E3 7E                  LD A,(HL)
1447   08E4 7E                  LD A,(HL)
1448   08E5 7E                  LD A,(HL)
1449   08E6 7E                  LD A,(HL)
1450   08E7 7E                  LD A,(HL)
1451   08E8 7E                  LD A,(HL)
1452   08E9 7E                  LD A,(HL)
1453   08EA 7E                  LD A,(HL)
1454   08EB 7E                  LD A,(HL)
1455   08EC 7E                  LD A,(HL)
1456   08ED 7E                  LD A,(HL)
1457   08EE 7E                  LD A,(HL)
1458   08EF 7E                  LD A,(HL)
1459   08F0 7E                  LD A,(HL)
1460   08F1 7E                  LD A,(HL)
1461   08F2 7E                  LD A,(HL)
1462   08F3 7E                  LD A,(HL)
1463   08F4 7E                  LD A,(HL)
1464   08F5 7E                  LD A,(HL)
1465   08F6 7E                  LD A,(HL)
1466   08F7 7E                  LD A,(HL)
1467   08F8 7E                  LD A,(HL)
1468   08F9 7E                  LD A,(HL)
1469   08FA 7E                  LD A,(HL)
1470   08FB 7E                  LD A,(HL)
1471   08FC 7E                  LD A,(HL)
1472   08FD 7E                  LD A,(HL)
1473   08FE 7E                  LD A,(HL)
1474   08FF 7E                  LD A,(HL)
1475   0900 7E                  LD A,(HL)
1476   0901 7E                  LD A,(HL)
1477   0902 7E                  LD A,(HL)
1478   0903 7E                  LD A,(HL)
1479   0904 7E                  LD A,(HL)
1480   0905 7E                  LD A,(HL)
1481   0906 7E                  LD A,(HL)
1482   0907 7E                  LD A,(HL)
1483   0908 7E                  LD A,(HL)
1484   0909 7E                  LD A,(HL)
1485   090A 7E                  LD A,(HL)
1486   090B 7E                  LD A,(HL)
1487   090C 7E                  LD A,(HL)
1488   090D 7E                  LD A,(HL)
1489   090E 7E                  LD A,(HL)
1490   090F 7E                  LD A,(HL)
1491   0910 7E                  LD A,(HL)
1492   0911 7E                  LD A,(HL)
1493   0912 7E                  LD A,(HL)
1494   0913 7E                  LD A,(HL)
1495   0914 7E                  LD A,(HL)
1496   0915 7E                  LD A,(HL)
1497   0916 7E                  LD A,(HL)
1498   0917 7E                  LD A,(HL)
1499   0918 7E                  LD A,(HL)
1500   0919 7E                  LD A,(HL)
1501   091A 7E                  LD A,(HL)
1502   091B 7E                  LD A,(HL)
1503   091C 7E                  LD A,(HL)
1504   091D 7E                  LD A,(HL)
1505   091E 7E                  LD A,(HL)
1506   091F 7E                  LD A,(HL)
1507   0920 7E                  LD A,(HL)
1508   0921 7E                  LD A,(HL)
1509   0922 7E                  LD A,(HL)
1510   0923 7E                  LD A,(HL)
1511   0924 7E                  LD A,(HL)
1512   0925 7E                  LD A,(HL)
1513   0926 7E                  LD A,(HL)
1514   0927 7E                  LD A,(HL)
1515   0928 7E                  LD A,(HL)
1516   0929 7E                  LD A,(HL)
1517   092A 7E                  LD A,(HL)
1518   092B 7E                  LD A,(HL)
1519   092C 7E                  LD A,(HL)
1520   092D 7E                  LD A,(HL)
1521   092E 7E                  LD A,(HL)
1522   092F 7E                  LD A,(HL)
1523   0930 7E                  LD A,(HL)
1524   0931 7E                  LD A,(HL)
1525   0932 7E                  LD A,(HL)
1526   0933 7E                  LD A,(HL)
1527   0934 7E                  LD A,(HL)
1528   0935 7E                  LD A,(HL)
1529   0936 7E                  LD A,(HL)
1530   0937 7E                  LD A,(HL)
1531   0938 7E                  LD A,(HL)
1532   0939 7E                  LD A,(HL)
1533   093A 7E                  LD A,(HL)
1534   093B 7E                  LD A,(HL)
1535   093C 7E                  LD A,(HL)
1536   093D 7E                  LD A,(HL)
1537   093E 7E                  LD A,(HL)
1538   093F 7E                  LD A,(HL)
1539   0940 7E                  LD A,(HL)
1540   0941 7E                  LD A,(HL)
1541   0942 7E                  LD A,(HL)
1542   0943 7E                  LD A,(HL)
1543   0944 7E                  LD A,(HL)
1544   0945 7E                  LD A,(HL)
1545   0946 7E                  LD A,(HL)
1546   0947 7E                  LD A,(HL)
1547   0948 7E                  LD A,(HL)
1548   0949 7E                  LD A,(HL)
1549   094A 7E                  LD A,(HL)
1550   094B 7E                  LD A,(HL)
1551   094C 7E                  LD A,(HL)
1552   094D 7E                  LD A,(HL)
1553   094E 7E                  LD A,(HL)
1554   094F 7E                  LD A,(HL)
1555   0950 7E                  LD A,(HL)
1556   0951 7E                  LD A,(HL)
1557   0952 7E                  LD A,(HL)
1558   0953 7E                  LD A,(HL)
1559   0954 7E                  LD A,(HL)
1560   0955 7E                  LD A,(HL)
1561   0956 7E                  LD A,(HL)
1562   0957 7E                  LD A,(HL)
1563   0958 7E                  LD A,(HL)
1564   0959 7E                  LD A,(HL)
1565   095A 7E                  LD A,(HL)
1566   095B 7E                  LD A,(HL)
1567   095C 7E                  LD A,(HL)
1568   095D 7E                  LD A,(HL)
1569   095E 7E                  LD A,(HL)
1570   095F 7E                  LD A,(HL)
1571   0960 7E                  LD A,(HL)
1572   0961 7E                  LD A,(HL)
1573   0962 7E                  LD A,(HL)
1574   0963 7E                  LD A,(HL)
1575   0964 7E                  LD A,(HL)
1576   0965 7E                  LD A,(HL)
1577   0966 7E                  LD A,(HL)
1578   0967 7E                  LD A,(HL)
1579   0968 7E                  LD A,(HL)
1580   0969 7E                  LD A,(HL)
1581   096A 7E                  LD A,(HL)
1582   096B 7E                  LD A,(HL)
1583   096C 7E                  LD A,(HL)
1584   096D 7E                  LD A,(HL)
1585   096E 7E                  LD A,(HL)
1586   096F 7E                  LD A,(HL)
1587   0970 7E                  LD A,(HL)
1588   0971 7E                  LD A,(HL)
1589   0972 7E                  LD A,(HL)
1590   0973 7E                  LD A,(HL)
1591   0974 7E                  LD A,(HL)
1592   0975 7E                  LD A,(HL)
1593   0976 7E                  LD A,(HL)
1594   0977 7E                  LD A,(HL)
1595   0978 7E                  LD A,(HL)
1596   0979 7E                  LD A,(HL)
1597   097A 7E                  LD A,(HL)
1598   097B 7E                  LD A,(HL)
1599   097C 7E                  LD A,(HL)
1600   097D 7E                  LD A,(HL)
1601   097E 7E                  LD A,(HL)
1602   097F 7E                  LD A,(HL)
1603   0980 7E                  LD A,(HL)
1604   0981 7E                  LD A,(HL)
1605   0982 7E                  LD A,(HL)
1606   0983 7E                  LD A,(HL)
1607   0984 7E                  LD A,(HL)
1608   0985 7E                  LD A,(HL)
1609   0986 7E                  LD A,(HL)
1610   0987 7E                  LD A,(HL)
1611   0988 7E                  LD A,(HL)
1612   0989 7E                  LD A,(HL)
1613   098A 7E                  LD A,(HL)
1614   098B 7E                  LD A,(HL)
1615   098C 7E                  LD A,(HL)
1616   098D 7E                  LD A,(HL)
1617   098E 7E                  LD A,(HL)
1618   098F 7E                  LD A,(HL)
1619   0990 7E                  LD A,(HL)
1620   0991 7E                  LD A,(HL)
1621   0992 7E                  LD A,(HL)
1622   0993 7E                  LD A,(HL)
1623   0994 7E                  LD A,(HL)
1624   0995 7E                  LD A,(HL)
1625   0996 7E                  LD A,(HL)
1626   0997 7E                  LD A,(HL)
1627   0998 7E                  LD A,(HL)
1628   0999 7E                  LD A,(HL)
1629   099A 7E                  LD A,(HL)
1630   099B 7E                  LD A,(HL)
1631   099C 7E                  LD A,(HL)
1632   099D 7E                  LD A,(HL)
1633   099E 7E                  LD A,(HL)
1634   099F 7E                  LD A,(HL)
1635   09A0 7E                  LD A,(HL)
1636   09A1 7E                  LD A,(HL)
1637   09A2 7E                  LD A,(HL)
1638   09A3 7E                  LD A,(HL)
1639   09A4 7E                  LD A,(HL)
1640   09A5 7E                  LD A,(HL)
1641   09A6 7E                  LD A,(HL)
1642   09A7 7E                  LD A,(HL)
1643   09A8 7E                  LD A,(HL)
1644   09A9 7E                  LD A,(HL)
1645   09AA 7E                  LD A,(HL)
1646   09AB 7E                  LD A,(HL)
1647   09AC 7E                  LD A,(HL)
1648   09AD 7E                  LD A,(HL)
1649   09AE 7E                  LD A,(HL)
1650   09AF 7E                  LD A,(HL)
1651   09B0 7E                  LD A,(HL)
1652   09B1 7E                  LD A,(HL)
1653   09B2 7E                  LD A,(HL)
1654   09B3 7E                  LD A,(HL)
1655   09B4 7E                  LD A,(HL)
1656   09B5 7E                  LD A,(HL)
1657   09B6 7E                  LD A,(HL)
1658   09B7 7E                  LD A,(HL)
1659   09B8 7E                  LD A,(HL)
1660   09B9 7E                  LD A,(HL)
1661   09BA 7E                  LD A,(HL)
1662   09BB 7E                  LD A,(HL)
1663   09BC 7E                  LD A,(HL)
1664   09BD 7E                  LD A,(HL)
1665   09BE 7E                  LD A,(HL)
1666   09BF 7E                  LD A,(HL)
1667   09C0 7E                  LD A,(HL)
1668   09C1 7E                  LD A,(HL)
1669   09C2 7E                  LD A,(HL)
1670   09C3 7E                  LD A,(HL)
1671   09C4 7E                  LD A,(HL)
1672   09C5 7E                  LD A,(HL)
1673   09C6 7E                  LD A,(HL)
1674   09C7 7E                  LD A,(HL)
1675   09C8 7E                  LD A,(HL)
1676   09C9 7E                  LD A,(HL)
1677   09CA 7E                  LD A,(HL)
1678   09CB 7E                  LD A,(HL)
1679   09CC 7E                  LD A,(HL)
1680   09CD 7E                  LD A,(HL)
1681   09CE 7E                  LD A,(HL)
1682   09CF 7E                  LD A,(HL)
1683   09D0 7E                  LD A,(HL)
1684   09D1 7E                  LD A,(HL)
1685   09D2 7E                  LD A,(HL)
1686   09D3 7E                  LD A,(HL)
1687   09D4 7E                  LD A,(HL)
1688   09D5 7E                  LD A,(HL)
1689   09D6 7E                  LD A,(HL)
1690   09D7 7E                  LD A,(HL)
1691   09D8 7E                  LD A,(HL)
1692   09D9 7E                  LD A,(HL)
1693   09DA 7E                  LD A,(HL)
1694   09DB 7E                  LD A,(HL)
1695   09DC 7E                  LD A,(HL)
1696   09DD 7E                  LD A,(HL)
1697   09DE 7E                  LD A,(HL)
1698   09DF 7E                  LD A,(HL)
1699   09E0 7E                  LD A,(HL)
1700   09E1 7E                  LD A,(HL)
1701   09E2 7E                  LD A,(HL)
1702   09E3 7E                  LD A,(HL)
1703   09E4 7E                  LD A,(HL)
1704   09E5 7E                  LD A,(HL)
1705   09E6 7E                  LD A,(HL)
1706   09E7 7E                  LD A,(HL)
1707   09E8 7E                  LD A,(HL)
1708   09E9 7E                  LD A,(HL)
1709   09EA 7E                  LD A,(HL)
1710   09EB 7E                  LD A,(HL)
1711   09EC 7E                  LD A,(HL)
1712   09ED 7E                  LD A,(HL)
1713   09EE 7E                  LD A,(HL)
1714   09EF 7E                  LD A,(HL)
1715   09F0 7E                  LD A,(HL)
1716   09F1 7E                  LD A,(HL)
1717   09F2 7E                  LD A,(HL)
1718   09F3 7E                  LD A,(HL)
1719   09F4 7E                  LD A,(HL)
1720   09F5 7E                  LD A,(HL)
1721   09F6 7E                  LD A,(HL)
1722   09F7 7E                  LD A,(HL)
1723   09F8 7E                  LD A,(HL)
1724   09F9 7E                  LD A,(HL)
1725   09FA 7E                  LD A,(HL)
1726   09FB 7E                  LD A,(HL)
1727   09FC 7E                  LD A,(HL)
1728   09FD 7E                  LD A,(HL)
1729   09FE 7E                  LD A,(HL)
1730   09FF 7E                  LD A,(HL)
1731   0A00 7E                  LD A,(HL)
1732   0A01 7E                  LD A,(HL)
1733   0A02 7E                  LD A,(HL)
1734   0A03 7E                  LD A,(HL)
1735   0A04 7E                  LD A,(HL)
1736   0A05 7E                  LD A,(HL)
1737   0A06 7E                  LD A,(HL)
1738   0A07 7E                  LD A,(HL)
1739   0A08 7E                  LD A,(HL)
1740   0A09 7E                  LD A,(HL)
1741   0A0A 7E                  LD A,(HL)
1742   0A0B 7E                  LD A,(HL)
1743   0A0C 7E                  LD A,(HL)
1744   0A0D 7E                  LD A,(HL)
1745   0A0E 7E                  LD A,(HL)
1746   0A0F 7E                  LD A,(HL)
1747   0A10 7E                  LD A,(HL)
1748   0A11 7E                  LD A,(HL)
1749   0A12 7E                  LD A,(HL)
1750   0A13 7E                  LD A,(HL)
1751   0A14 7E                  LD A,(HL)
1752   0A15 7E                  LD A,(HL)
1753   0A16 7E                  LD A,(HL)
1754   0A17 7E                  LD A,(HL)
1755   0A18 7E                  LD A,(HL)
1756   0A19 7E                  LD A,(HL)
1757   0A1A 7E                  LD A,(HL)
1758   0A1B 7E                  LD A,(HL)
1759   0A1C 7E                  LD A,(HL)
1760   0A1D 7E                  LD A,(HL)
1761   0A1E 7E                  LD A,(HL)
1762   0A1F 7E                  LD A,(HL)
1763   0A20 7E                  LD A,(HL)
1764   0A21 7E                  LD A,(HL)
1765   0A22 7E                  LD A,(HL)
1766   0A23 7E                  LD A,(HL)
1767   0A24 7E                  LD A,(HL)
1768   0A25 7E                  LD A,(HL)
1769   0A26 7E                  LD A,(HL)
1770   0A27 7E                  LD A,(HL)
1771   0A28 7E                  LD A,(HL)
1772   0A29 7E                  LD A,(HL)
1773   0A2A 7E                  LD A,(HL)
1774   0A2B 7E                  LD A,(HL)
1775   0A2C 7E                  LD A,(HL)
1776   0A2D 7E                  LD A,(HL)
1777   0A2E 7E                  LD A,(HL)
1778   0A2F 7E                  LD A,(HL)
1779   0A30 7E                  LD A,(HL)
1780   0A31 7E                  LD A,(HL)
1781   0A32 7E                  LD A,(HL)
1782   0A33 7E                  LD A,(HL)
1783   0A34 7E                  LD A,(HL)
1784   0A35 7E                  LD A,(HL)
1785   0A36 7E                  LD A,(HL)
1786   0A37 7E                  LD A,(HL)
1787   0A38 7E                  LD A,(HL)
1788   0A39 7E                  LD A,(HL)
1789   0A3A 7E                  LD A,(HL)
1790   0A3B 7E                  LD A,(HL)
1791   0A3C 7E                  LD A,(HL)
1792   0A3D 7E                  LD A,(HL)
1793   0A3E 7E                  LD A,(HL)
1794   0A3F 7E                  LD A,(HL)
1795   0A40 7E                  LD A,(HL)
1796   0A41 7E                  LD A,(HL)
1797   0A42 7E                  LD A,(HL)
1798   0A43 7E                  LD A,(HL)
1799   0A44 7E                  LD A,(HL)
1800   0A45 7E                  LD A,(HL)
1801   0A46 7E                  LD A,(HL)
1802   0A47 7E                  LD A,(HL)
1803   0A48 7E                  LD A,(HL)
1804   0A49 7E                  LD A,(HL)
1805   0A4A 7E                  LD A,(HL)
1806   0A4B 7E                  LD A,(HL)
1807   0A4C 7E                  LD A,(HL)
1808   0A4D 7E                  LD A,(HL)
1809   0A4E 7E                  LD A,(HL)
1810   0A4F 7E                  LD A,(HL)
1811   0A50 7E                  LD A,(HL)
1812   0A51 7E                  LD A,(HL)
1813   0A52 7E                  LD A,(HL)
1814   0A53 7E                  LD A,(HL)
1815   0A54 7E                  LD A,(HL)
1816   0A55 7E                  LD A,(HL)
1817   0A56 7E                  LD A,(HL)
1818   0A57 7E                  LD A,(HL)
1819   0A58 7E                  LD A,(HL)
1820   0A59 7E                  LD A,(HL)
1821   0A5A 7E                  LD A,(HL)
1822   0A5B 7E                  LD A,(HL)
1823   0A5C 7E                  LD A,(HL)
1824   0A5D 7E                  LD A,(HL)
1825   0A5E 7E                  LD A,(HL)
1826   0A5F 7E                  LD A,(HL)
1827   0A60 7E                  LD A,(HL)
1828   0A61 7E                  LD A,(HL)
1829   0A62 7E                  LD A,(HL)
1830   0A63 7E                  LD A,(HL)
1831   0A64 7E                  LD A,(HL)
1832   0A65 7E                  LD A,(HL)
1833   0A66 7E                  LD A,(HL)
1834   0A67 7E                  LD A,(HL)
1835   0A68 7E                  LD A,(HL)
1836   0A69 7E                  LD A,(HL)
1837   0A6A 7E                  LD A,(HL)
1838   0A6B 7E                  LD A,(HL)
1839   0A6C 7E                  LD A,(HL)
1840   0A6D 7E                  LD A,(HL)
1841   0A6E 7E                  LD A,(HL)
1842   0A6F 7E                  LD A,(HL)
1843   0A70 7E                  LD A,(HL)
1844   0A71 7E                  LD A,(HL)
1845   0A72 7E                  LD A,(HL)
1846   0A73 7E                  LD A,(HL)
1847   0A74 7E                  LD A,(HL)
1848   0A75 7E                  LD A,(HL)
1849   0A76 7E                  LD A,(HL)
1850   0A77 7E                  LD A,(HL)
1851   0A78 7E                  LD A,(HL)
1852   0A79 7E                  LD A,(HL)
1853   0A7A 7E                  LD A,(HL)
1854   0A7B 7E                  LD A,(HL)
1855   0A7C 7E                  LD A,(HL)
1856   0A7D 7E                  LD A,(HL)
1857   0A7E 7E                  LD A,(HL)
1858   0A7F 7E                  LD A,(HL)
1859   0A80 7E                  LD A,(HL)
1860   0A81 7E                  LD A,(HL)
1861   0A82 7E                  LD A,(HL)
1862   0A83 7E                  LD A,(HL)
1863   0A84 7E                  LD A,(HL)
1864   0A85 7E                  LD A,(HL)
1865   0A86 7E                  LD A,(HL)
1866   0A87 7E                  LD A,(HL)
1867   0A88 7E                  LD A,(HL)
1868   0A89 7E                  LD A,(HL)
1869   0A8A 7E                  LD A,(HL)
1870   0A8B 7E                  LD A,(HL)
1871   0A8C 7E                  LD A,(HL)
1872   0A8D 7E                  LD A,(HL)
1873   0A8E 7E                  LD A,(HL)
1874   0A8F 7E                  LD A,(HL)
1875   0A90 7E                  LD A,(HL)
1876   0A91 7E                  LD A,(HL)
1877   0A92 7E                  LD A,(HL)
1878   0A93 7E                  LD A,(HL)
1879   0A94 7E                  LD A,(HL)
1880   0A95 7E                  LD A,(HL)
1881   0A96 7E                  LD A,(HL)
1882   0A97 7E                  LD A,(HL)
1883   0A98 7E                  LD A,(HL)
1884   0A99 7E                  LD A,(HL)
1885   0A9A 7E                  LD A,(HL)
1886   0A9B 7E                  LD A,(HL)
1887   0A9C 7E                  LD A,(HL)
1888   0A9D 7E                  LD A,(HL)
1889   0A9E 7E                  LD A,(HL)
1890   0A9F 7E                  LD A,(HL)
1891   0AA0 7E                  LD A,(HL)
1892   0AA1 7E                  LD A,(HL)
1893   0AA2 7E                  LD A,(HL)
1894   0AA3 7E                  LD A,(HL)
1895   0AA4 7E                  LD A,(HL)
1896   0AA5 7E                  LD A,(HL)
1897   0AA6 7E                  LD A,(HL)
1898   0AA7 7E                  LD A,(HL)
1899   0AA8 7E                  LD A,(HL)
1900   0AA9 7E                  LD A,(HL)
1901   0AAA 7E                  LD A,(HL)
1902   0AAB 7E                  LD A,(HL)
1903   0AAC 7E                  LD A,(HL)
1904   0AAD 7E                  LD A,(HL)
1905   0AAE 7E                  LD A,(HL)
1906   0AAF 7E                  LD A,(HL)
1907   0AB0 7E                  LD A,(HL)
1908   0AB1 7E                  LD A,(HL)
1909   0AB2 7E                  LD A,(HL)
1910   0AB3 7E                  LD A,(HL)
1911   0AB4 7E                  LD A,(HL)
1912   0AB5 7E                  LD A,(HL)
1913   0AB6 7E                  LD A,(HL)
1914   0AB7 7E                  LD A,(HL)
1915   0AB8 7E                  LD A,(HL)
1916   0AB9 7E                  LD A,(HL)
1917   0ABA 7E                  LD A,(HL)
1918   0ABB 7E                  LD A,(HL)
1919   0ABC 7E                  LD A,(HL)
1920   0ABD 7E                  LD A,(HL)
1921   0ABE 7E                  LD A,(HL)
1922   0ABF 7E                  LD A,(HL)
1923   0AC0 7E                  LD A,(HL)
1924   0AC1 7E                  LD A,(HL)
1925   0AC2 7E                  LD A,(HL)
1926   0AC3 7E                  LD A,(HL)
1927   0AC4 7E                  LD A,(HL)
1928   0AC5 7E                  LD A,(HL)
1929   0AC6 7E                  LD A,(HL)
1930   0AC7 7E                  LD A,(HL)
1931   0AC8 7E                  LD A,(HL)
1932   0AC9 7E                  LD A,(HL)
1933   0ACA 7E                  LD A,(HL)
1934   0ACB 7E                  LD A,(HL)
1935   0ACC 7E                  LD A,(HL)
1936   0ACD 7E                  LD A,(HL)
1937   0ACE 7E                  LD A,(HL)
1938   0ACF 7E                  LD A,(HL)
1939   0AD0 7E                  LD A,(HL)
1940   0AD1 7E                  LD A,(HL)
1941   0AD2 7E                  LD A,(HL)
1942   0AD3 7E                  LD A,(HL)
1943   0AD4 7E                  LD A,(HL)
1944   0AD5 7E                  LD A,(HL)
1945   0AD6 7E                  LD A,(HL)
1946   0AD7 7E                  LD A,(HL)
1947   0AD8 7E                  LD A,(HL)
1948   0AD9 7E                  LD A,(HL)
1949   0ADA 7E                  LD A,(HL)
1950   0ADB 7E                  LD A,(HL)
1951   0ADC 7E                  LD A,(HL)
1952   0ADD 7E                  LD A,(HL)
1953   0ADE 7E                  LD A,(HL)
1954   0ADF 7E                  LD A,(HL)
1955   0AE0 7E                  LD A,(HL)
1956   0AE1 7E                  LD A,(HL)
1957   0AE2 7E                  LD A,(HL)
1958   0AE3 7E                  LD A,(HL)
1959   0AE4 7E                  LD A,(HL)
1960   0AE5 7E                  LD A,(HL)
1961   0AE6 7E                  LD A,(HL)
1962   0AE7 7E                  LD A,(HL)
1963   0AE8 7E                  LD A,(HL)
1964   0AE9 7E                  LD A,(HL)
1965   0AEA 7E                  LD A,(HL)
1966   0AEB 7E                  LD A,(HL)
1967   0AEC 7E                  LD A,(HL)
1968   0AED 7E                  LD A,(HL)
1969   0AEE 7E                  LD A,(HL)
1970   0AEF 7E                  LD A,(HL)

---------------------------------------------
LIST OF LABELS
Sorted by address:      Sorted by name:
---------------------------------------------

L0006:  0003            L0001:  016A
L0007:  001D            L0002:  00BC
L0008:  0079            L0003:  00A9
L0009:  0090            L0004:  009F
L0010:  0099            L0005:  02B2
L0011:  009B            L0006:  0003
L0004:  009F            L0007:  001D
L0003:  00A9            L0008:  0079
L0012:  00AF            L0009:  0090
L0013:  00B0            L0010:  0099
L0002:  00BC            L0011:  009B
L0027:  00F4            L0012:  00AF
L0017:  0104            L0013:  00B0
L0016:  0111            L0014:  01FD
L0018:  0122            L0015:  01FA
L0020:  0127            L0016:  0111
L0019:  0135            L0017:  0104
L0022:  013A            L0018:  0122
L0021:  014E            L0019:  0135
L0001:  016A            L0020:  0127
L0024:  0188            L0021:  014E
L0023:  01A6            L0022:  013A
L0015:  01FA            L0023:  01A6
L0014:  01FD            L0024:  0188
L0035:  0208            L0025:  0219
L0037:  0211            L0026:  023D
L0036:  0214            L0027:  00F4
L0025:  0219            L0028:  02B5
L0032:  0221            L0029:  02A6
L0033:  0227            L0030:  022B
L0030:  022B            L0031:  0244
L0026:  023D            L0032:  0221
L0031:  0244            L0033:  0227
L0034:  0256            L0034:  0256
L0038:  0258            L0035:  0208
L0040:  0273            L0036:  0214
L0045:  029E            L0037:  0211
L0029:  02A6            L0038:  0258
L0005:  02B2            L0039:  3000
L0028:  02B5            L0040:  0273
L0044:  02C8            L0041:  02CD
L0042:  02CC            L0042:  02CC
L0041:  02CD            L0043:  02E0
L0043:  02E0            L0044:  02C8
L0051:  02EF            L0045:  029E
L0053:  02F0            L0046:  C904
L0052:  02FD            L0047:  0486
L0068:  0309            L0048:  04FE
L0069:  0329            L0049:  055F
L0062:  0332            L0050:  048C
L0059:  034C            L0051:  02EF
L0058:  0363            L0052:  02FD
L0060:  0370            L0053:  02F0
L0061:  03A3            L0054:  03A5
L0054:  03A5            L0055:  0414
L0066:  03B0            L0056:  03E2
L0067:  03BB            L0057:  03FE
L0065:  03D1            L0058:  0363
L0064:  03D3            L0059:  034C
L0056:  03E2            L0060:  0370
L0057:  03FE            L0061:  03A3
L0055:  0414            L0062:  0332
L0073:  043C            L0063:  04B9
L0072:  044E            L0064:  03D3
L0071:  0453            L0065:  03D1
L0047:  0486            L0066:  03B0
L0050:  048C            L0067:  03BB
L0074:  04A2            L0068:  0309
L0079:  04A5            L0069:  0329
L0081:  04AA            L0070:  0689
L0078:  04AD            L0071:  0453
L0077:  04AF            L0072:  044E
L0063:  04B9            L0073:  043C
L0080:  04C4            L0074:  04A2
L0048:  04FE            L0075:  0692
L0087:  050F            L0076:  F600
L0085:  051B            L0077:  04AF
L0082:  052F            L0078:  04AD
L0083:  053E            L0079:  04A5
L0084:  0546            L0080:  04C4
L0049:  055F            L0081:  04AA
L0101:  0565            L0082:  052F
L0094:  057D            L0083:  053E
L0095:  059C            L0084:  0546
L0090:  059F            L0085:  051B
L0088:  05A4            L0086:  05C3
L0089:  05A7            L0087:  050F
L0091:  05B4            L0088:  05A4
L0086:  05C3            L0089:  05A7
L0102:  05D0            L0090:  059F
L0105:  05E5            L0091:  05B4
L0096:  05EA            L0092:  05F5
L0103:  05EC            L0093:  061A
L0092:  05F5            L0094:  057D
L0108:  05F8            L0095:  059C
L0097:  05F9            L0096:  05EA
L0098:  0601            L0097:  05F9
L0114:  0608            L0098:  0601
L0099:  060A            L0099:  060A
L0109:  060D            L0100:  068F
L0115:  0617            L0101:  0565
L0093:  061A            L0102:  05D0
L0110:  0627            L0103:  05EC
L0106:  0637            L0104:  0661
L0111:  0643            L0105:  05E5
L0112:  0647            L0106:  0637
L0107:  0653            L0107:  0653
L0104:  0661            L0108:  05F8
L0113:  0664            L0109:  060D
L0070:  0689            L0110:  0627
L0100:  068F            L0111:  0643
L0075:  0692            L0112:  0647
L0118:  06D4            L0113:  0664
L0116:  06E6            L0114:  0608
L0117:  06F4            L0115:  0617
L0129:  0702            L0116:  06E6
L0123:  0714            L0117:  06F4
L0121:  071B            L0118:  06D4
L0128:  071F            L0119:  0733
L0120:  0724            L0120:  0724
L0125:  0726            L0121:  071B
L0124:  0729            L0122:  073A
L0119:  0733            L0123:  0714
L0122:  073A            L0124:  0729
L0126:  073E            L0125:  0726
L0127:  0749            L0126:  073E
L0140:  0788            L0127:  0749
L0130:  0799            L0128:  071F
L0131:  07A0            L0129:  0702
L0132:  07A7            L0130:  0799
L0133:  07A8            L0131:  07A0
L0136:  07BC            L0132:  07A7
L0134:  07BD            L0133:  07A8
L0135:  07C1            L0134:  07BD
L0138:  07C2            L0135:  07C1
L0139:  07C7            L0136:  07BC
L0137:  07C8            L0137:  07C8
L0039:  3000            L0138:  07C2
L0046:  C904            L0139:  07C7
L0076:  F600            L0140:  0788
