
(cl:in-package :asdf)

(defsystem "jimmy-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils :std_msgs-msg
)
  :components ((:file "_package")
    (:file "jimmy_command" :depends-on ("_package_jimmy_command"))
    (:file "_package_jimmy_command" :depends-on ("_package"))
  ))