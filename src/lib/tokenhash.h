/* C++ code produced by gperf version 3.1 */
/* Command-line: gperf --compare-strncmp -C -m 20 --output-file tokenhash.h tokens.gperf  */
/* Computed positions: -k'1,3,7,$' */

#if !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
      && ('%' == 37) && ('&' == 38) && ('\'' == 39) && ('(' == 40) \
      && (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) \
      && ('-' == 45) && ('.' == 46) && ('/' == 47) && ('0' == 48) \
      && ('1' == 49) && ('2' == 50) && ('3' == 51) && ('4' == 52) \
      && ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) \
      && ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) \
      && ('=' == 61) && ('>' == 62) && ('?' == 63) && ('A' == 65) \
      && ('B' == 66) && ('C' == 67) && ('D' == 68) && ('E' == 69) \
      && ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) \
      && ('J' == 74) && ('K' == 75) && ('L' == 76) && ('M' == 77) \
      && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) \
      && ('R' == 82) && ('S' == 83) && ('T' == 84) && ('U' == 85) \
      && ('V' == 86) && ('W' == 87) && ('X' == 88) && ('Y' == 89) \
      && ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) \
      && ('^' == 94) && ('_' == 95) && ('a' == 97) && ('b' == 98) \
      && ('c' == 99) && ('d' == 100) && ('e' == 101) && ('f' == 102) \
      && ('g' == 103) && ('h' == 104) && ('i' == 105) && ('j' == 106) \
      && ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) \
      && ('o' == 111) && ('p' == 112) && ('q' == 113) && ('r' == 114) \
      && ('s' == 115) && ('t' == 116) && ('u' == 117) && ('v' == 118) \
      && ('w' == 119) && ('x' == 120) && ('y' == 121) && ('z' == 122) \
      && ('{' == 123) && ('|' == 124) && ('}' == 125) && ('~' == 126))
/* The character set is not based on ISO-646.  */
#error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gperf@gnu.org>."
#endif

#line 5 "tokens.gperf"
struct xmltoken {
  const char *name;
  int tokenId;
};

#define TOTAL_KEYWORDS 262
#define MIN_WORD_LENGTH 1
#define MAX_WORD_LENGTH 23
#define MIN_HASH_VALUE 15
#define MAX_HASH_VALUE 551
/* maximum key range = 537, duplicates = 0 */

class Perfect_Hash
{
private:
  static inline unsigned int hash (const char *str, size_t len);
public:
  static const struct xmltoken *in_word_set (const char *str, size_t len);
};

inline unsigned int
Perfect_Hash::hash (const char *str, size_t len)
{
  static const unsigned short asso_values[] =
    {
      552, 552, 552, 552, 552, 552, 552, 552, 552, 552,
      552, 552, 552, 552, 552, 552, 552, 552, 552, 552,
      552, 552, 552, 552, 552, 552, 552, 552, 552, 552,
      552, 552, 552, 552, 552, 552, 552, 552, 552, 552,
      552, 552, 552, 552, 552, 552, 552, 552, 552, 552,
       12,   4, 552, 552, 552, 552, 552, 552, 552, 552,
      552, 552, 552, 552, 552,   4,   4,   9, 552,  56,
      552,   6,  12, 552,   5, 552,   5, 552, 552,   6,
      263, 552,   4, 552,  16, 552,   5,   3,   4,   3,
      552, 552, 552, 552, 552, 552, 552,  44, 107,  70,
       75,   6,  97,  79,  95,  33,   5,  34, 140, 175,
       33,  12,  17, 552,   3,   7,   3, 180, 166, 241,
       74, 166,  50, 552, 552, 552, 552, 552, 552, 552,
      552, 552, 552, 552, 552, 552, 552, 552, 552, 552,
      552, 552, 552, 552, 552, 552, 552, 552, 552, 552,
      552, 552, 552, 552, 552, 552, 552, 552, 552, 552,
      552, 552, 552, 552, 552, 552, 552, 552, 552, 552,
      552, 552, 552, 552, 552, 552, 552, 552, 552, 552,
      552, 552, 552, 552, 552, 552, 552, 552, 552, 552,
      552, 552, 552, 552, 552, 552, 552, 552, 552, 552,
      552, 552, 552, 552, 552, 552, 552, 552, 552, 552,
      552, 552, 552, 552, 552, 552, 552, 552, 552, 552,
      552, 552, 552, 552, 552, 552, 552, 552, 552, 552,
      552, 552, 552, 552, 552, 552, 552, 552, 552, 552,
      552, 552, 552, 552, 552, 552, 552, 552, 552, 552,
      552, 552, 552, 552, 552, 552
    };
  unsigned int hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[static_cast<unsigned char>(str[6])];
      /*FALLTHROUGH*/
      case 6:
      case 5:
      case 4:
      case 3:
        hval += asso_values[static_cast<unsigned char>(str[2])];
      /*FALLTHROUGH*/
      case 2:
      case 1:
        hval += asso_values[static_cast<unsigned char>(str[0])];
        break;
    }
  return hval + asso_values[static_cast<unsigned char>(str[len - 1])];
}

static const struct xmltoken wordlist[] =
  {
    {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
    {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
    {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
    {(char*)0}, {(char*)0}, {(char*)0},
#line 244 "tokens.gperf"
    {"target",XML_TARGET},
    {(char*)0},
#line 172 "tokens.gperf"
    {"or",XML_OR},
#line 88 "tokens.gperf"
    {"entryY",XML_ENTRYY},
#line 87 "tokens.gperf"
    {"entryX",XML_ENTRYX},
#line 72 "tokens.gperf"
    {"east",XML_EAST},
    {(char*)0},
#line 209 "tokens.gperf"
    {"root",XML_ROOT},
    {(char*)0}, {(char*)0}, {(char*)0},
#line 166 "tokens.gperf"
    {"object",XML_OBJECT},
#line 187 "tokens.gperf"
    {"part",XML_PART},
#line 202 "tokens.gperf"
    {"resizeLast",XML_RESIZELAST},
#line 186 "tokens.gperf"
    {"parent",XML_PARENT},
#line 243 "tokens.gperf"
    {"tape",XML_TAPE},
#line 73 "tokens.gperf"
    {"eastwest",XML_EASTWEST},
#line 234 "tokens.gperf"
    {"stepPerimeter",XML_STEPPERIMETER},
#line 235 "tokens.gperf"
    {"strokeColor",XML_STROKECOLOR},
#line 233 "tokens.gperf"
    {"step",XML_STEP},
#line 188 "tokens.gperf"
    {"perimeter",XML_PERIMETER},
#line 206 "tokens.gperf"
    {"rhombus",XML_RHOMBUS},
#line 201 "tokens.gperf"
    {"resizeHeight",XML_RESIZEHEIGHT},
#line 177 "tokens.gperf"
    {"outlineConnect",XML_OUTLINECONNECT},
    {(char*)0},
#line 251 "tokens.gperf"
    {"top",XML_TOP},
#line 207 "tokens.gperf"
    {"rhombusPerimeter",XML_RHOMBUSPERIMETER},
#line 84 "tokens.gperf"
    {"entityRelationEdgeStyle",XML_ENTITYRELATIONEDGESTYLE},
    {(char*)0},
#line 213 "tokens.gperf"
    {"separatorColor",XML_SEPARATORCOLOR},
    {(char*)0},
#line 164 "tokens.gperf"
    {"note",XML_NOTE},
#line 250 "tokens.gperf"
    {"tooltips",XML_TOOLTIPS},
    {(char*)0},
#line 176 "tokens.gperf"
    {"orthogonalPerimeter",XML_ORTHOGONALPERIMETER},
#line 195 "tokens.gperf"
    {"process",XML_PROCESS},
#line 185 "tokens.gperf"
    {"parallelogramPerimeter",XML_PARALLELOGRAMPERIMETER},
#line 174 "tokens.gperf"
    {"orthogonalEdgeStyle",XML_ORTHOGONALEDGESTYLE},
#line 18 "tokens.gperf"
    {"as",XML_AS},
    {(char*)0},
#line 169 "tokens.gperf"
    {"open",XML_OPEN},
#line 139 "tokens.gperf"
    {"jettySize",XML_JETTYSIZE},
#line 194 "tokens.gperf"
    {"position2",XML_POSITION2},
#line 175 "tokens.gperf"
    {"orthogonalLoop",XML_ORTHOGONALLOOP},
#line 211 "tokens.gperf"
    {"rotation",XML_ROTATION},
#line 17 "tokens.gperf"
    {"arrows",XML_ARROWS},
#line 161 "tokens.gperf"
    {"noJump",XML_NOJUMP},
#line 215 "tokens.gperf"
    {"shape",XML_SHAPE},
#line 190 "tokens.gperf"
    {"points",XML_POINTS},
    {(char*)0}, {(char*)0}, {(char*)0},
#line 216 "tokens.gperf"
    {"size",XML_SIZE},
    {(char*)0}, {(char*)0},
#line 191 "tokens.gperf"
    {"portConstraint",XML_PORTCONSTRAINT},
    {(char*)0},
#line 138 "tokens.gperf"
    {"isometricEdgeStyle",XML_ISOMETRICEDGESTYLE},
#line 189 "tokens.gperf"
    {"pointerEvents",XML_POINTEREVENTS},
    {(char*)0},
#line 10 "tokens.gperf"
    {"absoluteArcSize",XML_ABSOLUTEARCSIZE},
#line 163 "tokens.gperf"
    {"none",XML_NONE},
#line 193 "tokens.gperf"
    {"position",XML_POSITION},
    {(char*)0},
#line 93 "tokens.gperf"
    {"ERone",XML_ERONE},
#line 253 "tokens.gperf"
    {"trapezoidPerimeter",XML_TRAPEZOIDPERIMETER},
    {(char*)0}, {(char*)0},
#line 270 "tokens.gperf"
    {"xor",XML_XOR},
#line 248 "tokens.gperf"
    {"text",XML_TEXT},
#line 38 "tokens.gperf"
    {"circle",XML_CIRCLE},
    {(char*)0},
#line 238 "tokens.gperf"
    {"swimlane",XML_SWIMLANE},
#line 131 "tokens.gperf"
    {"image",XML_IMAGE},
    {(char*)0},
#line 208 "tokens.gperf"
    {"right",XML_RIGHT},
#line 242 "tokens.gperf"
    {"swimlaneLine",XML_SWIMLANELINE},
#line 171 "tokens.gperf"
    {"openThin",XML_OPENTHIN},
#line 240 "tokens.gperf"
    {"swimlaneFillColor",XML_SWIMLANEFILLCOLOR},
#line 50 "tokens.gperf"
    {"cross",XML_CROSS},
#line 74 "tokens.gperf"
    {"edge",XML_EDGE},
#line 140 "tokens.gperf"
    {"jiggle",XML_JIGGLE},
#line 134 "tokens.gperf"
    {"imageHeight",XML_IMAGEHEIGHT},
#line 133 "tokens.gperf"
    {"imageAspect",XML_IMAGEASPECT},
#line 232 "tokens.gperf"
    {"startSize",XML_STARTSIZE},
#line 83 "tokens.gperf"
    {"endSize",XML_ENDSIZE},
#line 137 "tokens.gperf"
    {"internalStorage",XML_INTERNALSTORAGE},
    {(char*)0},
#line 69 "tokens.gperf"
    {"dropTarget",XML_DROPTARGET},
#line 97 "tokens.gperf"
    {"expand",XML_EXPAND},
    {(char*)0},
#line 180 "tokens.gperf"
    {"page",XML_PAGE},
#line 60 "tokens.gperf"
    {"dataStorage",XML_DATASTORAGE},
#line 192 "tokens.gperf"
    {"portConstraintRotation",XML_PORTCONSTRAINTROTATION},
#line 229 "tokens.gperf"
    {"stackLayout",XML_STACKLAYOUT},
#line 129 "tokens.gperf"
    {"id",XML_ID},
#line 20 "tokens.gperf"
    {"autosize",XML_AUTOSIZE},
#line 35 "tokens.gperf"
    {"center",XML_CENTER},
#line 119 "tokens.gperf"
    {"group",XML_GROUP},
#line 217 "tokens.gperf"
    {"sketch",XML_SKETCH},
#line 11 "tokens.gperf"
    {"align",XML_ALIGN},
#line 47 "tokens.gperf"
    {"connect",XML_CONNECT},
    {(char*)0},
#line 167 "tokens.gperf"
    {"offset",XML_OFFSET},
#line 205 "tokens.gperf"
    {"resizeWidth",XML_RESIZEWIDTH},
    {(char*)0}, {(char*)0}, {(char*)0},
#line 48 "tokens.gperf"
    {"connectable",XML_CONNECTABLE},
#line 24 "tokens.gperf"
    {"base",XML_BASE},
#line 120 "tokens.gperf"
    {"guides",XML_GUIDES},
#line 249 "tokens.gperf"
    {"textDirection",XML_TEXTDIRECTION},
#line 198 "tokens.gperf"
    {"recursiveResize",XML_RECURSIVERESIZE},
#line 210 "tokens.gperf"
    {"rotatable",XML_ROTATABLE},
#line 58 "tokens.gperf"
    {"dashPattern",XML_DASHPATTERN},
    {(char*)0}, {(char*)0},
#line 200 "tokens.gperf"
    {"resizable",XML_RESIZABLE},
#line 14 "tokens.gperf"
    {"arcSize",XML_ARCSIZE},
#line 135 "tokens.gperf"
    {"imageVerticalAlign",XML_IMAGEVERTICALALIGN},
#line 114 "tokens.gperf"
    {"glass",XML_GLASS},
#line 165 "tokens.gperf"
    {"north",XML_NORTH},
#line 122 "tokens.gperf"
    {"height",XML_HEIGHT},
#line 130 "tokens.gperf"
    {"ignoreEdge",XML_IGNOREEDGE},
#line 95 "tokens.gperf"
    {"ERzeroToOne",XML_ERZEROTOONE},
#line 106 "tokens.gperf"
    {"flipV",XML_FLIPV},
    {(char*)0},
#line 181 "tokens.gperf"
    {"pageHeight",XML_PAGEHEIGHT},
#line 252 "tokens.gperf"
    {"trapezoid",XML_TRAPEZOID},
#line 226 "tokens.gperf"
    {"spacingLeft",XML_SPACINGLEFT},
#line 227 "tokens.gperf"
    {"spacingRight",XML_SPACINGRIGHT},
#line 55 "tokens.gperf"
    {"darkOpacity2",XML_DARKOPACITY2},
#line 105 "tokens.gperf"
    {"flipH",XML_FLIPH},
#line 49 "tokens.gperf"
    {"container",XML_CONTAINER},
#line 269 "tokens.gperf"
    {"x",XML_X},
    {(char*)0},
#line 70 "tokens.gperf"
    {"dx",XML_DX},
#line 34 "tokens.gperf"
    {"card",XML_CARD},
#line 66 "tokens.gperf"
    {"direction",XML_DIRECTION},
#line 52 "tokens.gperf"
    {"curved",XML_CURVED},
#line 182 "tokens.gperf"
    {"pageScale",XML_PAGESCALE},
    {(char*)0},
#line 228 "tokens.gperf"
    {"spacingTop",XML_SPACINGTOP},
#line 27 "tokens.gperf"
    {"block",XML_BLOCK},
    {(char*)0},
#line 241 "tokens.gperf"
    {"swimlaneHead",XML_SWIMLANEHEAD},
#line 37 "tokens.gperf"
    {"childLayout",XML_CHILDLAYOUT},
#line 43 "tokens.gperf"
    {"cloud",XML_CLOUD},
#line 57 "tokens.gperf"
    {"dashed",XML_DASHED},
#line 85 "tokens.gperf"
    {"entryDx",XML_ENTRYDX},
#line 80 "tokens.gperf"
    {"ellipse",XML_ELLIPSE},
    {(char*)0},
#line 196 "tokens.gperf"
    {"rectangle",XML_RECTANGLE},
    {(char*)0}, {(char*)0}, {(char*)0},
#line 81 "tokens.gperf"
    {"ellipsePerimeter",XML_ELLIPSEPERIMETER},
#line 115 "tokens.gperf"
    {"gradientColor",XML_GRADIENTCOLOR},
#line 197 "tokens.gperf"
    {"rectanglePerimeter",XML_RECTANGLEPERIMETER},
    {(char*)0}, {(char*)0},
#line 118 "tokens.gperf"
    {"gridSize",XML_GRIDSIZE},
#line 173 "tokens.gperf"
    {"orthogonal",XML_ORTHOGONAL},
#line 15 "tokens.gperf"
    {"Array",XML_ARRAY},
    {(char*)0},
#line 127 "tokens.gperf"
    {"horizontalStack",XML_HORIZONTALSTACK},
#line 56 "tokens.gperf"
    {"dash",XML_DASH},
    {(char*)0}, {(char*)0},
#line 237 "tokens.gperf"
    {"style",XML_STROKESTYLE},
#line 61 "tokens.gperf"
    {"default",XML_DEFAULT},
    {(char*)0},
#line 51 "tokens.gperf"
    {"cube",XML_CUBE},
#line 107 "tokens.gperf"
    {"flowAnimation",XML_FLOWANIMATION},
#line 67 "tokens.gperf"
    {"document",XML_DOCUMENT},
#line 256 "tokens.gperf"
    {"triangle",XML_TRIANGLE},
#line 117 "tokens.gperf"
    {"grid",XML_GRID},
#line 264 "tokens.gperf"
    {"visible",XML_VISIBLE},
#line 77 "tokens.gperf"
    {"editable",XML_EDITABLE},
#line 111 "tokens.gperf"
    {"fontSize",XML_FONTSIZE},
#line 236 "tokens.gperf"
    {"strokeOpacity",XML_STROKEOPACITY},
#line 257 "tokens.gperf"
    {"trianglePerimeter",XML_TRIANGLEPERIMETER},
    {(char*)0}, {(char*)0},
#line 219 "tokens.gperf"
    {"source",XML_SOURCE},
#line 178 "tokens.gperf"
    {"oval",XML_OVAL},
#line 150 "tokens.gperf"
    {"message",XML_MESSAGE},
    {(char*)0}, {(char*)0},
#line 42 "tokens.gperf"
    {"cloneable",XML_CLONEABLE},
#line 12 "tokens.gperf"
    {"allowArrows",XML_ALLOWARROWS},
#line 116 "tokens.gperf"
    {"gradientDirection",XML_GRADIENTDIRECTION},
    {(char*)0},
#line 79 "tokens.gperf"
    {"elbowEdgeStyle",XML_ELBOWEDGESTYLE},
#line 125 "tokens.gperf"
    {"hidden",XML_HIDDEN},
    {(char*)0},
#line 147 "tokens.gperf"
    {"link",XML_LINK},
#line 23 "tokens.gperf"
    {"backgroundOutline",XML_BACKGROUNDOUTLINE},
    {(char*)0},
#line 184 "tokens.gperf"
    {"parallelogram",XML_PARALLELOGRAM},
#line 136 "tokens.gperf"
    {"imageWidth",XML_IMAGEWIDTH},
#line 224 "tokens.gperf"
    {"spacing",XML_SPACING},
    {(char*)0}, {(char*)0},
#line 102 "tokens.gperf"
    {"fixedSize",XML_FIXEDSIZE},
    {(char*)0},
#line 124 "tokens.gperf"
    {"hexagonPerimeter",XML_HEXAGONPERIMETER},
#line 151 "tokens.gperf"
    {"metaEdit",XML_METAEDIT},
#line 32 "tokens.gperf"
    {"callout",XML_CALLOUT},
#line 25 "tokens.gperf"
    {"baseDash",XML_BASEDASH},
    {(char*)0}, {(char*)0}, {(char*)0},
#line 41 "tokens.gperf"
    {"classicThin",XML_CLASSICTHIN},
#line 53 "tokens.gperf"
    {"cylinder3",XML_CYLINDER3},
#line 21 "tokens.gperf"
    {"backbonePerimeter",XML_BACKBONEPERIMETER},
    {(char*)0},
#line 33 "tokens.gperf"
    {"calloutPerimeter",XML_CALLOUTPERIMETER},
#line 231 "tokens.gperf"
    {"startFill",XML_STARTFILL},
#line 45 "tokens.gperf"
    {"collapsible",XML_COLLAPSIBLE},
    {(char*)0}, {(char*)0}, {(char*)0},
#line 65 "tokens.gperf"
    {"diamondThin",XML_DIAMONDTHIN},
#line 254 "tokens.gperf"
    {"treeFolding",XML_TREEFOLDING},
    {(char*)0}, {(char*)0},
#line 123 "tokens.gperf"
    {"hexagon",XML_HEXAGON},
#line 71 "tokens.gperf"
    {"dy",XML_DY},
#line 145 "tokens.gperf"
    {"left",XML_LEFT},
#line 89 "tokens.gperf"
    {"enumerate",XML_ENUMERATE},
    {(char*)0}, {(char*)0}, {(char*)0},
#line 260 "tokens.gperf"
    {"vertex",XML_VERTEX},
#line 90 "tokens.gperf"
    {"enumerateValue",XML_ENUMERATEVALUE},
#line 239 "tokens.gperf"
    {"swimlaneBody",XML_SWIMLANEBODY},
#line 91 "tokens.gperf"
    {"ERmandOne",XML_ERMANDONE},
    {(char*)0},
#line 121 "tokens.gperf"
    {"halfCircle",XML_HALFCIRCLE},
#line 265 "tokens.gperf"
    {"west",XML_WEST},
#line 28 "tokens.gperf"
    {"blockThin",XML_BLOCKTHIN},
#line 94 "tokens.gperf"
    {"ERoneToMany",XML_ERONETOMANY},
#line 31 "tokens.gperf"
    {"box",XML_BOX},
#line 262 "tokens.gperf"
    {"verticalAlign",XML_VERTICALALIGN},
#line 132 "tokens.gperf"
    {"imageAlign",XML_IMAGEALIGN},
#line 40 "tokens.gperf"
    {"classic",XML_CLASSIC},
#line 152 "tokens.gperf"
    {"middle",XML_MIDDLE},
#line 170 "tokens.gperf"
    {"openAsync",XML_OPENASYNC},
#line 255 "tokens.gperf"
    {"treeMoving",XML_TREEMOVING},
    {(char*)0},
#line 76 "tokens.gperf"
    {"edgeStyle",XML_EDGESTYLE},
#line 263 "tokens.gperf"
    {"verticalLabelPosition",XML_VERTICALLABELPOSITION},
#line 113 "tokens.gperf"
    {"geometry",XML_GEOMETRY},
    {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
    {(char*)0},
#line 22 "tokens.gperf"
    {"background",XML_BACKGROUND},
#line 183 "tokens.gperf"
    {"pageWidth",XML_PAGEWIDTH},
#line 64 "tokens.gperf"
    {"diamond",XML_DIAMOND},
#line 149 "tokens.gperf"
    {"math",XML_MATH},
#line 143 "tokens.gperf"
    {"labelBorderColor",XML_LABELBORDERCOLOR},
#line 179 "tokens.gperf"
    {"overflow",XML_OVERFLOW},
    {(char*)0},
#line 126 "tokens.gperf"
    {"horizontal",XML_HORIZONTAL},
#line 109 "tokens.gperf"
    {"fontColor",XML_FONTCOLOR},
#line 247 "tokens.gperf"
    {"targetPoint",XML_TARGETPOINT},
#line 157 "tokens.gperf"
    {"mxfile",XML_MXFILE},
#line 19 "tokens.gperf"
    {"async",XML_ASYNC},
    {(char*)0},
#line 223 "tokens.gperf"
    {"south",XML_SOUTH},
#line 203 "tokens.gperf"
    {"resizeParent",XML_RESIZEPARENT},
    {(char*)0},
#line 146 "tokens.gperf"
    {"lifelinePerimeter",XML_LIFELINEPERIMETER},
#line 29 "tokens.gperf"
    {"bottom",XML_BOTTOM},
#line 246 "tokens.gperf"
    {"targetPortConstraint",XML_TARGETPORTCONSTRAINT},
#line 16 "tokens.gperf"
    {"arrow",XML_ARROW},
#line 26 "tokens.gperf"
    {"bendable",XML_BENDABLE},
    {(char*)0}, {(char*)0}, {(char*)0},
#line 214 "tokens.gperf"
    {"shadow",XML_SHADOW},
#line 54 "tokens.gperf"
    {"darkOpacity",XML_DARKOPACITY},
#line 96 "tokens.gperf"
    {"ERzeroToMany",XML_ERZEROTOMANY},
#line 44 "tokens.gperf"
    {"collapsed",XML_COLLAPSED},
    {(char*)0}, {(char*)0},
#line 68 "tokens.gperf"
    {"doubleBlock",XML_DOUBLEBLOCK},
#line 230 "tokens.gperf"
    {"startArrow",XML_STARTARROW},
#line 268 "tokens.gperf"
    {"wrap",XML_WRAP},
#line 267 "tokens.gperf"
    {"whiteSpace",XML_WHITESPACE},
    {(char*)0},
#line 103 "tokens.gperf"
    {"fixedWidth",XML_FIXEDWIDTH},
    {(char*)0},
#line 112 "tokens.gperf"
    {"fontStyle",XML_FONTSTYLE},
    {(char*)0}, {(char*)0},
#line 142 "tokens.gperf"
    {"labelBackgroundColor",XML_LABELBACKGROUNDCOLOR},
    {(char*)0},
#line 108 "tokens.gperf"
    {"fold",XML_FOLD},
#line 259 "tokens.gperf"
    {"value",XML_VALUE},
#line 225 "tokens.gperf"
    {"spacingBottom",XML_SPACINGBOTTOM},
    {(char*)0},
#line 46 "tokens.gperf"
    {"comic",XML_COMIC},
    {(char*)0}, {(char*)0},
#line 199 "tokens.gperf"
    {"relative",XML_RELATIVE},
    {(char*)0},
#line 162 "tokens.gperf"
    {"noLabel",XML_NOLABEL},
    {(char*)0}, {(char*)0},
#line 218 "tokens.gperf"
    {"snapToPoint",XML_SNAPTOPOINT},
    {(char*)0},
#line 156 "tokens.gperf"
    {"mxCell",XML_MXCELL},
    {(char*)0}, {(char*)0},
#line 271 "tokens.gperf"
    {"y",XML_Y},
    {(char*)0}, {(char*)0}, {(char*)0},
#line 62 "tokens.gperf"
    {"deletable",XML_DELETABLE},
    {(char*)0}, {(char*)0},
#line 212 "tokens.gperf"
    {"rounded",XML_ROUNDED},
#line 75 "tokens.gperf"
    {"edgeLabel",XML_EDGELABEL},
#line 82 "tokens.gperf"
    {"endArrow",XML_ENDARROW},
#line 258 "tokens.gperf"
    {"umlActor",XML_UMLACTOR},
    {(char*)0}, {(char*)0},
#line 59 "tokens.gperf"
    {"data:image/png",XML_DATA_IMAGE_PNG},
    {(char*)0},
#line 86 "tokens.gperf"
    {"entryDy",XML_ENTRYDY},
    {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 39 "tokens.gperf"
    {"circlePlus",XML_CIRCLEPLUS},
    {(char*)0}, {(char*)0},
#line 104 "tokens.gperf"
    {"flexArrow",XML_FLEXARROW},
    {(char*)0}, {(char*)0},
#line 78 "tokens.gperf"
    {"elbow",XML_ELBOW},
#line 153 "tokens.gperf"
    {"movable",XML_MOVABLE},
#line 261 "tokens.gperf"
    {"vertical",XML_VERTICAL},
#line 204 "tokens.gperf"
    {"resizeParentMax",XML_RESIZEPARENTMAX},
#line 158 "tokens.gperf"
    {"mxGeometry",XML_MXGEOMETRY},
    {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 101 "tokens.gperf"
    {"fixDash",XML_FIXDASH},
#line 148 "tokens.gperf"
    {"marginBottom",XML_MARGINBOTTOM},
#line 245 "tokens.gperf"
    {"targetPerimeterSpacing",XML_TARGETPERIMETERSPACING},
    {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
    {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
    {(char*)0}, {(char*)0},
#line 98 "tokens.gperf"
    {"fill",XML_FILL},
#line 144 "tokens.gperf"
    {"labelPadding",XML_LABELPADDING},
    {(char*)0},
#line 36 "tokens.gperf"
    {"centerPerimeter",XML_CENTERPERIMETER},
    {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 99 "tokens.gperf"
    {"fillColor",XML_FILLCOLOR},
    {(char*)0}, {(char*)0},
#line 141 "tokens.gperf"
    {"label",XML_LABEL},
    {(char*)0}, {(char*)0},
#line 168 "tokens.gperf"
    {"opacity",XML_OPACITY},
    {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
    {(char*)0}, {(char*)0}, {(char*)0},
#line 92 "tokens.gperf"
    {"ERmany",XML_ERMANY},
    {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
    {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
    {(char*)0}, {(char*)0},
#line 128 "tokens.gperf"
    {"html",XML_HTML},
    {(char*)0},
#line 266 "tokens.gperf"
    {"width",XML_WIDTH},
    {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
    {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
    {(char*)0}, {(char*)0}, {(char*)0},
#line 159 "tokens.gperf"
    {"mxGraphModel",XML_MXGRAPHMODEL},
    {(char*)0},
#line 13 "tokens.gperf"
    {"anchorPointDirection",XML_ANCHORPOINTDIRECTION},
    {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
    {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
    {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
    {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
    {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 160 "tokens.gperf"
    {"mxPoint",XML_MXPOINT},
    {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
    {(char*)0}, {(char*)0},
#line 100 "tokens.gperf"
    {"fillOpacity",XML_FILLOPACITY},
    {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
    {(char*)0},
#line 222 "tokens.gperf"
    {"sourcePoint",XML_SOURCEPOINT},
    {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
    {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 221 "tokens.gperf"
    {"sourcePortConstraint",XML_SOURCEPORTCONSTRAINT},
    {(char*)0}, {(char*)0},
#line 63 "tokens.gperf"
    {"diagram",XML_DIAGRAM},
    {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 110 "tokens.gperf"
    {"fontFamily",XML_FONTFAMILY},
    {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
    {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
    {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
    {(char*)0}, {(char*)0}, {(char*)0},
#line 155 "tokens.gperf"
    {"moveCells",XML_MOVECELLS},
    {(char*)0},
#line 154 "tokens.gperf"
    {"movableLabel",XML_MOVABLELABEL},
    {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
    {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
    {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
#line 30 "tokens.gperf"
    {"boundedLbl",XML_BOUNDEDLBL},
    {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
    {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
    {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
    {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
    {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
    {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
    {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
    {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
    {(char*)0}, {(char*)0}, {(char*)0}, {(char*)0},
    {(char*)0}, {(char*)0},
#line 220 "tokens.gperf"
    {"sourcePerimeterSpacing",XML_SOURCEPERIMETERSPACING}
  };

const struct xmltoken *
Perfect_Hash::in_word_set (const char *str, size_t len)
{
  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      unsigned int key = hash (str, len);

      if (key <= MAX_HASH_VALUE)
        {
          const char *s = wordlist[key].name;

          if (s && *str == *s && !strncmp (str + 1, s + 1, len - 1) && s[len] == '\0')
            return &wordlist[key];
        }
    }
  return 0;
}
