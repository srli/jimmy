; Auto-generated. Do not edit!


(cl:in-package jimmy-msg)


;//! \htmlinclude jimmy_servo.msg.html

(cl:defclass <jimmy_servo> (roslisp-msg-protocol:ros-message)
  ((servo_names
    :reader servo_names
    :initarg :servo_names
    :type (cl:vector cl:string)
   :initform (cl:make-array 0 :element-type 'cl:string :initial-element ""))
   (positions
    :reader positions
    :initarg :positions
    :type (cl:vector cl:float)
   :initform (cl:make-array 0 :element-type 'cl:float :initial-element 0.0)))
)

(cl:defclass jimmy_servo (<jimmy_servo>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <jimmy_servo>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'jimmy_servo)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name jimmy-msg:<jimmy_servo> is deprecated: use jimmy-msg:jimmy_servo instead.")))

(cl:ensure-generic-function 'servo_names-val :lambda-list '(m))
(cl:defmethod servo_names-val ((m <jimmy_servo>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader jimmy-msg:servo_names-val is deprecated.  Use jimmy-msg:servo_names instead.")
  (servo_names m))

(cl:ensure-generic-function 'positions-val :lambda-list '(m))
(cl:defmethod positions-val ((m <jimmy_servo>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader jimmy-msg:positions-val is deprecated.  Use jimmy-msg:positions instead.")
  (positions m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <jimmy_servo>) ostream)
  "Serializes a message object of type '<jimmy_servo>"
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'servo_names))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (cl:let ((__ros_str_len (cl:length ele)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_str_len) ostream))
  (cl:map cl:nil #'(cl:lambda (c) (cl:write-byte (cl:char-code c) ostream)) ele))
   (cl:slot-value msg 'servo_names))
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'positions))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (cl:let ((bits (roslisp-utils:encode-double-float-bits ele)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 32) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 40) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 48) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 56) bits) ostream)))
   (cl:slot-value msg 'positions))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <jimmy_servo>) istream)
  "Deserializes a message object of type '<jimmy_servo>"
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'servo_names) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'servo_names)))
    (cl:dotimes (i __ros_arr_len)
    (cl:let ((__ros_str_len 0))
      (cl:setf (cl:ldb (cl:byte 8 0) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:aref vals i) (cl:make-string __ros_str_len))
      (cl:dotimes (__ros_str_idx __ros_str_len msg)
        (cl:setf (cl:char (cl:aref vals i) __ros_str_idx) (cl:code-char (cl:read-byte istream))))))))
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'positions) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'positions)))
    (cl:dotimes (i __ros_arr_len)
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 32) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 40) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 48) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 56) bits) (cl:read-byte istream))
    (cl:setf (cl:aref vals i) (roslisp-utils:decode-double-float-bits bits))))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<jimmy_servo>)))
  "Returns string type for a message object of type '<jimmy_servo>"
  "jimmy/jimmy_servo")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'jimmy_servo)))
  "Returns string type for a message object of type 'jimmy_servo"
  "jimmy/jimmy_servo")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<jimmy_servo>)))
  "Returns md5sum for a message object of type '<jimmy_servo>"
  "38811bf91898e543ed7e7f543da886c8")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'jimmy_servo)))
  "Returns md5sum for a message object of type 'jimmy_servo"
  "38811bf91898e543ed7e7f543da886c8")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<jimmy_servo>)))
  "Returns full string definition for message of type '<jimmy_servo>"
  (cl:format cl:nil "string[] servo_names~%float64[] positions~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'jimmy_servo)))
  "Returns full string definition for message of type 'jimmy_servo"
  (cl:format cl:nil "string[] servo_names~%float64[] positions~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <jimmy_servo>))
  (cl:+ 0
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'servo_names) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ 4 (cl:length ele))))
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'positions) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ 8)))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <jimmy_servo>))
  "Converts a ROS message object to a list"
  (cl:list 'jimmy_servo
    (cl:cons ':servo_names (servo_names msg))
    (cl:cons ':positions (positions msg))
))
