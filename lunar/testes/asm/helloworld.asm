# always initialize registers that your program may use
# if you don't your program may have unexpected behavior
# one-letter subroutine names are allowed in NexFUSE but not OpenLUD
# in the future, there may be fewer restrictions and multiple-letter
# subroutines will be allowed
@M:
    init R1
    
    mov R1, 3

    each R1

    halt
