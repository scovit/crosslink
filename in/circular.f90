PROGRAM main
USE shared_parameter

IMPLICIT NONE

REAL(DBL) :: b = 1
INTEGER :: nbead
INTEGER :: nlinks
REAL(DBL),ALLOCATABLE,DIMENSION(:,:):: r
INTEGER,ALLOCATABLE,DIMENSION(:) :: laplace
CHARACTER(len=32) :: config_file, beads_arg
INTEGER :: j, status, config_int
REAL(DBL) :: radius

CALL get_command_argument(1,VALUE=beads_arg,STATUS=status)
IF (status /= 0) STOP 'No number of beads specified'
CALL get_command_argument(2,VALUE=config_file,STATUS=status)
IF (status /= 0) STOP 'No correct config file designated'

READ(beads_arg,'(i10)') nbead
READ(config_file,'(i10)') config_int

OPEN(10,FILE='./io/pos'//trim(config_file)//'.dat', &
     ACTION='WRITE',STATUS='UNKNOWN')
OPEN(14,FILE='./io/lpl'//trim(config_file)//'.dat', &
     ACTION='WRITE',STATUS='UNKNOWN')

 ! initialize in straight line with initial spring extension b                 
   ! nbead = 21
   ALLOCATE(r(3,nbead),STAT=status)

   radius = (nbead - 1) * 2.d0 * b / 2 / pi
   r(2,:)= (/(radius * COS(DBLE(j) * 2.d0 * pi / (nbead)),j=0,nbead-1)/)
   r(1,:)= (/(radius * SIN(DBLE(j) * 2.d0 * pi / (nbead)),j=0,nbead-1)/)
   r(3,:)= 0.d0

 ! circular polymer
   
   nlinks = nbead * 2
   ALLOCATE(laplace(nbead + nlinks), STAT=status)
   laplace(1) = 2
   laplace(2) = nbead
   laplace(3) = 2
   DO j=2,nbead-1
      laplace((j - 2) * 3 + 4) = 2
      laplace((j - 2) * 3 + 5) = j - 1
      laplace((j - 2) * 3 + 6) = j + 1
   END DO
   laplace(4 + (nbead - 2) * 3) = 2
   laplace(4 + (nbead - 2) * 3 + 1) = nbead - 1
   laplace(4 + (nbead - 2) * 3 + 2) = 1

WRITE(10, *) nbead
WRITE(10, *) r
WRITE(14, *) nlinks
WRITE(14, *) laplace

CLOSE(10)
CLOSE(14)
END PROGRAM main
