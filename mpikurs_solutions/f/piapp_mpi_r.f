      program piapp
      implicit none
      include 'mpif.h'

      double precision  pia, pial, piexact
      integer  i, ip, n
      double precision del, app, xi
      integer comm, np, me, ierr
      piexact = 3.14159265358979323846264338327950288419d0 

! 1) set up mpi environment
      call MPI_INIT( ierr )
      comm = MPI_COMM_WORLD
      call MPI_COMM_SIZE( comm, np, ierr )
      call MPI_COMM_RANK( comm, me, ierr )
!  input of n
! 2) for me=0 : read the value for n from standard input
      if (me.eq.0) then
         write(6,*)'number of intervals ?  '
         read(5,*) n
!         n = 100000000
         write(6,1001) n, np
 1001 format(//'calculation of pi using numerical integration of '
     :         'SQRT(1-x**2)'
     :        /'number of intervals    : ',I20,
     :        /'number of processes    : ',I20)
      end if
! 3) distribute n to all tasks
      if (me.eq.0) then
         do ip = 1 , np-1
            call MPI_SEND( n, 1, MPI_INTEGER, ip, 0, comm, ierr )
         end do
      else 
         call MPI_RECV( n, 1, MPI_INTEGER, 0, 0,
     :                  comm, MPI_STATUS_IGNORE,ierr )
      end if
! 4) starting from strip 1+me sum every np-th strip on task me 
      del = 1.d0/dble(n)
      app = 0.0d0
      do i = me+1 , n, np
         xi =  (dble(i)-0.5d0)*del
         app = app + sqrt(1.d0-xi*xi)
      end do
      pial = 4.d0*del*app

! 5) collect all partial sums on task 0
      if (me.eq.0) then
         pia = pial      
         do ip = 1 , np-1
            call MPI_RECV(pial, 1, MPI_DOUBLE_PRECISION, ip, ip,
     :                    comm, MPI_STATUS_IGNORE, ierr )
            pia = pia + pial
         end do
      else
         call MPI_SEND( pial, 1, MPI_DOUBLE_PRECISION, 0, me,
     :                  comm, ierr )
      end if

! 6) output of results
      if (me.eq.0) then
         write(6,1111)piexact, pia, abs(pia-piexact)
 1111 format(/'pi exact              = ',f20.14,
     :       /'pi estimated          = ',f20.14,
     :       /'error                 = ',e20.3)
      end if
      call MPI_FINALIZE(ierr)
      stop
      end
