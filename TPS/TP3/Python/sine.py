sine = [189,190,192,194,196,197,199,201,
203,205,206,208,210,212,214,215,
217,219,221,222,224,226,228,229,
231,233,235,236,238,240,242,243,
245,247,248,250,252,254,255,257,
259,260,262,264,265,267,268,270,
272,273,275,276,278,280,281,283,
284,286,287,289,290,292,293,295,
296,298,299,301,302,304,305,306,
308,309,311,312,313,315,316,317,
319,320,321,322,324,325,326,327,
329,330,331,332,333,334,336,337,
338,339,340,341,342,343,344,345,
346,347,348,349,350,351,352,353,
354,354,355,356,357,358,358,359,
360,361,361,362,363,363,364,365,
365,366,367,367,368,368,369,369,
370,370,371,371,372,372,372,373,
373,374,374,374,375,375,375,375,
376,376,376,376,376,376,377,377,
377,377,377,377,377,377,377,377,
377,377,377,377,377,376,376,376,
376,376,376,375,375,375,375,374,
374,374,373,373,372,372,372,371,
371,370,370,369,369,368,368,367,
367,366,365,365,364,363,363,362,
361,361,360,359,358,358,357,356,
355,354,354,353,352,351,350,349,
348,347,346,345,344,343,342,341,
340,339,338,337,336,334,333,332,
331,330,329,327,326,325,324,322,
321,320,319,317,316,315,313,312,
311,309,308,306,305,304,302,301,
299,298,296,295,293,292,290,289,
287,286,284,283,281,280,278,276,
275,273,272,270,268,267,265,264,
262,260,259,257,255,254,252,250,
248,247,245,243,242,240,238,236,
235,233,231,229,228,226,224,222,
221,219,217,215,214,212,210,208,
206,205,203,201,199,197,196,194,
192,190,189,187,185,183,181,180,
178,176,174,172,171,169,167,165,
163,162,160,158,156,155,153,151,
149,148,146,144,142,141,139,137,
135,134,132,130,129,127,125,123,
122,120,118,117,115,113,112,110,
109,107,105,104,102,101,99,97,
96,94,93,91,90,88,87,85,
84,82,81,79,78,76,75,73,
72,71,69,68,66,65,64,62,
61,60,58,57,56,55,53,52,
51,50,48,47,46,45,44,43,
41,40,39,38,37,36,35,34,
33,32,31,30,29,28,27,26,
25,24,23,23,22,21,20,19,
19,18,17,16,16,15,14,14,
13,12,12,11,10,10,9,9,
8,8,7,7,6,6,5,5,
5,4,4,3,3,3,2,2,
2,2,1,1,1,1,1,1,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,1,
1,1,1,1,1,2,2,2,
2,3,3,3,4,4,5,5,
5,6,6,7,7,8,8,9,
9,10,10,11,12,12,13,14,
14,15,16,16,17,18,19,19,
20,21,22,23,23,24,25,26,
27,28,29,30,31,32,33,34,
35,36,37,38,39,40,41,43,
44,45,46,47,48,50,51,52,
53,55,56,57,58,60,61,62,
64,65,66,68,69,71,72,73,
75,76,78,79,81,82,84,85,
87,88,90,91,93,94,96,97,
99,101,102,104,105,107,109,110,
112,113,115,117,118,120,122,123,
125,127,129,130,132,134,135,137,
139,141,142,144,146,148,149,151,
153,155,156,158,160,162,163,165,
167,169,171,172,174,176,178,180,
181,183,185,187,189]

sine2 = [(sine[i] + 1) for i in range(len(sine))]

for i in range(len(sine2)//8):
        print(sine2[i*8],',', sine2[i*8+1], ',',sine2[i*8+2], ',',sine2[i*8+3],
              ',',sine2[i*8+4],',',sine2[i*8+5],
              ',',sine2[i*8+6],',', sine2[i*8+7], ',')