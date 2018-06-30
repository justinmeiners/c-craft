
#ifndef ENDIAN_H
#define ENDIAN_H

#include <stdint.h>
#include <assert.h>

#define ENDIAN_LITTLE 1

/* is the current platform little or big endian? */
extern int End_IsLittle();

/* little endian */

extern void End_U16ToLittle(void* dst, uint16_t val);
extern void End_I16ToLittle(void* dst, int16_t val);

extern void End_U32ToLittle(void* dst, uint32_t val);
extern void End_I32ToLittle(void* dst, int32_t val);

extern void End_U64ToLittle(void* dst, uint64_t val);
extern void End_I64ToLittle(void* dst, int64_t val);

extern uint16_t End_U16FromLittle(const void* src);
extern int16_t End_I16FromLittle(const void* src);

extern uint32_t End_U32FromLittle(const void* src);
extern int32_t End_I32FromLittle(const void* src);

extern uint64_t End_U64FromLittle(const void* src);
extern int64_t End_I64FromLittle(const void* src);

extern void End_F32ToLittle(void* dst, float val);
extern float End_F32FromLittle(const void* src);

/* big endian */

extern void End_U16ToBig(void* dst, uint16_t val);
extern void End_I16ToBig(void* dst, int16_t val);

extern void End_U32ToBig(void* dst, uint32_t val);
extern void End_I32ToBig(void* dst, int32_t val);

extern void End_U64ToBig(void* dst, uint64_t val);
extern void End_I64ToBig(void* dst, int64_t val);

extern uint16_t End_U16FromBig(const void* src);
extern int16_t End_I16FromBig(const void* src);

extern uint32_t End_U32FromBig(const void* src);
extern int32_t End_I32FromBig(const void* src);

extern uint64_t End_U64FromBig(const void* src);
extern int64_t End_I64FromBig(const void* src);

extern void End_F32ToBig(void* dst, float val);
extern float End_F32FromBig(const void* src);


#define End_U16Write(_dst, _val, _littleEndian) \
(_littleEndian) ? End_U16ToLittle(_dst, _val) : End_U16ToBig(_dst, _val)

#define End_I16Write(_dst, _val, _littleEndian) \
(_littleEndian) ? End_I16ToLittle(_dst, _val) : End_I16ToBig(_dst, _val)

#define End_U32Write(_dst, _val, _littleEndian) \
(_littleEndian) ? End_U32ToLittle(_dst, _val) : End_U32ToBig(_dst, _val)

#define End_I32Write(_dst, _val, _littleEndian) \
(_littleEndian) ? End_I32ToLittle(_dst, _val) : End_I32ToBig(_dst, _val)

#define End_U64Write(_dst, _val, _littleEndian) \
(_littleEndian) ? End_U64ToLittle(_dst, _val) : End_U64ToBig(_dst, _val)

#define End_I64Write(_dst, _val, _littleEndian) \
(_littleEndian) ? End_I64ToLittle(_dst, _val) : End_I64ToBig(_dst, _val)

#define End_U16Read(_src, _littleEndian) \
(_littleEndian) ? End_U16FromLittle(_src) : End_U16FromBig(_src)

#define End_I16Read(_src, _littleEndian) \
(_littleEndian) ? End_I16FromLittle(_src) : End_I16FromBig(_src)

#define End_U32Read(_src, _littleEndian) \
(_littleEndian) ? End_U32FromLittle(_src) : End_U32FromBig(_src)

#define End_I32Read(_src, _littleEndian) \
(_littleEndian) ? End_I32FromLittle(_src) : End_I32FromBig(_src)

#define End_U64Read(_src, _littleEndian) \
(_littleEndian) ? End_U32FromLittle(_src) : End_U64FromBig(_src)

#define End_I64Read(_src, _littleEndian) \
(_littleEndian) ? End_I32FromLittle(_src) : End_I64FromBig(_src)

#endif
