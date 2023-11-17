# always initialize registers that your program may use
# if you don't your program may have unexpected behavior
# one-letter subroutine names are allowed in NexFUSE but not OpenLUD
# in the future, there may be fewer restrictions and multiple-letter
# subroutines will be allowed
@M:
    init R21
    null
    mov R1, 3
    null
    each R1
    null
    halt
