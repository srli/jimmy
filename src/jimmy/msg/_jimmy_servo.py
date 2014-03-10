"""autogenerated by genpy from jimmy/jimmy_servo.msg. Do not edit."""
import sys
python3 = True if sys.hexversion > 0x03000000 else False
import genpy
import struct


class jimmy_servo(genpy.Message):
  _md5sum = "a12069c278d2ca26c24908f496f4482f"
  _type = "jimmy/jimmy_servo"
  _has_header = False #flag to mark the presence of a Header object
  _full_text = """int32 ID_R_SHOULDER_PITCH      = 16
int32 ID_L_SHOULDER_PITCH      = 12
int32 ID_R_SHOULDER_ROLL       = 17
int32 ID_L_SHOULDER_ROLL       = 13
int32 ID_R_ELBOW               = 19
int32 ID_L_ELBOW               = 15
int32 ID_R_HIP_YAW             = 6
int32 ID_L_HIP_YAW             = 0
int32 ID_R_HIP_ROLL            = 8
int32 ID_L_HIP_ROLL            = 2
int32 ID_R_HIP_PITCH           = 7
int32 ID_L_HIP_PITCH           = 1
int32 ID_R_KNEE                = 9
int32 ID_L_KNEE                = 3
int32 ID_R_ANKLE_PITCH         = 10
int32 ID_L_ANKLE_PITCH         = 4
int32 ID_R_ANKLE_ROLL          = 11
int32 ID_L_ANKLE_ROLL          = 5
int32 ID_HEAD_PAN              = 20
int32 ID_HEAD_TILT             = 21
int32 ID_R_ELBOW_YAW           = 18
int32 ID_L_ELBOW_YAW           = 14
int32 ID_R_WRIST_YAW           = 23
int32 ID_L_WRIST_YAW           = 24
int32 ID_R_GRIPPER             = 25
int32 ID_L_GRIPPER             = 26
int32 ID_HEAD_TILT_2           = 22

int32[] servo_numbers
float64[] positions
"""
  # Pseudo-constants
  ID_R_SHOULDER_PITCH = 16
  ID_L_SHOULDER_PITCH = 12
  ID_R_SHOULDER_ROLL = 17
  ID_L_SHOULDER_ROLL = 13
  ID_R_ELBOW = 19
  ID_L_ELBOW = 15
  ID_R_HIP_YAW = 6
  ID_L_HIP_YAW = 0
  ID_R_HIP_ROLL = 8
  ID_L_HIP_ROLL = 2
  ID_R_HIP_PITCH = 7
  ID_L_HIP_PITCH = 1
  ID_R_KNEE = 9
  ID_L_KNEE = 3
  ID_R_ANKLE_PITCH = 10
  ID_L_ANKLE_PITCH = 4
  ID_R_ANKLE_ROLL = 11
  ID_L_ANKLE_ROLL = 5
  ID_HEAD_PAN = 20
  ID_HEAD_TILT = 21
  ID_R_ELBOW_YAW = 18
  ID_L_ELBOW_YAW = 14
  ID_R_WRIST_YAW = 23
  ID_L_WRIST_YAW = 24
  ID_R_GRIPPER = 25
  ID_L_GRIPPER = 26
  ID_HEAD_TILT_2 = 22

  __slots__ = ['servo_numbers','positions']
  _slot_types = ['int32[]','float64[]']

  def __init__(self, *args, **kwds):
    """
    Constructor. Any message fields that are implicitly/explicitly
    set to None will be assigned a default value. The recommend
    use is keyword arguments as this is more robust to future message
    changes.  You cannot mix in-order arguments and keyword arguments.

    The available fields are:
       servo_numbers,positions

    :param args: complete set of field values, in .msg order
    :param kwds: use keyword arguments corresponding to message field names
    to set specific fields.
    """
    if args or kwds:
      super(jimmy_servo, self).__init__(*args, **kwds)
      #message fields cannot be None, assign default values for those that are
      if self.servo_numbers is None:
        self.servo_numbers = []
      if self.positions is None:
        self.positions = []
    else:
      self.servo_numbers = []
      self.positions = []

  def _get_types(self):
    """
    internal API method
    """
    return self._slot_types

  def serialize(self, buff):
    """
    serialize message into buffer
    :param buff: buffer, ``StringIO``
    """
    try:
      length = len(self.servo_numbers)
      buff.write(_struct_I.pack(length))
      pattern = '<%si'%length
      buff.write(struct.pack(pattern, *self.servo_numbers))
      length = len(self.positions)
      buff.write(_struct_I.pack(length))
      pattern = '<%sd'%length
      buff.write(struct.pack(pattern, *self.positions))
    except struct.error as se: self._check_types(struct.error("%s: '%s' when writing '%s'" % (type(se), str(se), str(_x))))
    except TypeError as te: self._check_types(ValueError("%s: '%s' when writing '%s'" % (type(te), str(te), str(_x))))

  def deserialize(self, str):
    """
    unpack serialized message in str into this message instance
    :param str: byte array of serialized message, ``str``
    """
    try:
      end = 0
      start = end
      end += 4
      (length,) = _struct_I.unpack(str[start:end])
      pattern = '<%si'%length
      start = end
      end += struct.calcsize(pattern)
      self.servo_numbers = struct.unpack(pattern, str[start:end])
      start = end
      end += 4
      (length,) = _struct_I.unpack(str[start:end])
      pattern = '<%sd'%length
      start = end
      end += struct.calcsize(pattern)
      self.positions = struct.unpack(pattern, str[start:end])
      return self
    except struct.error as e:
      raise genpy.DeserializationError(e) #most likely buffer underfill


  def serialize_numpy(self, buff, numpy):
    """
    serialize message with numpy array types into buffer
    :param buff: buffer, ``StringIO``
    :param numpy: numpy python module
    """
    try:
      length = len(self.servo_numbers)
      buff.write(_struct_I.pack(length))
      pattern = '<%si'%length
      buff.write(self.servo_numbers.tostring())
      length = len(self.positions)
      buff.write(_struct_I.pack(length))
      pattern = '<%sd'%length
      buff.write(self.positions.tostring())
    except struct.error as se: self._check_types(struct.error("%s: '%s' when writing '%s'" % (type(se), str(se), str(_x))))
    except TypeError as te: self._check_types(ValueError("%s: '%s' when writing '%s'" % (type(te), str(te), str(_x))))

  def deserialize_numpy(self, str, numpy):
    """
    unpack serialized message in str into this message instance using numpy for array types
    :param str: byte array of serialized message, ``str``
    :param numpy: numpy python module
    """
    try:
      end = 0
      start = end
      end += 4
      (length,) = _struct_I.unpack(str[start:end])
      pattern = '<%si'%length
      start = end
      end += struct.calcsize(pattern)
      self.servo_numbers = numpy.frombuffer(str[start:end], dtype=numpy.int32, count=length)
      start = end
      end += 4
      (length,) = _struct_I.unpack(str[start:end])
      pattern = '<%sd'%length
      start = end
      end += struct.calcsize(pattern)
      self.positions = numpy.frombuffer(str[start:end], dtype=numpy.float64, count=length)
      return self
    except struct.error as e:
      raise genpy.DeserializationError(e) #most likely buffer underfill

_struct_I = genpy.struct_I
