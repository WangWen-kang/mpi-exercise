      program synch_s
      implicit none
!  FORTRAN MPI-INCLUDE-file
      include 'mpif.h'
      integer    ierr, nproc, myid, ip, tag
      integer msg, i
      real*8 t1, t2, tbmax
!  initialize MPI, get nproc (# of procs) and myid (local proc. id)
      call MPI_INIT( ierr )
      call MPI_COMM_SIZE( MPI_COMM_WORLD, nproc, ierr )
      call MPI_COMM_RANK( MPI_COMM_WORLD, myid, ierr )
      call MPI_BARRIER( MPI_COMM_WORLD, ierr )


      t1 = MPI_WTIME()  
      t1 = MPI_WTIME()  
      msg = 1


! Example implementation of a barrier (sequential access):
!   all tasks except task 0 send a message ( msg ) to task 0
      do ip = 1 , nproc -1
         if (myid.eq.ip) then
             call MPI_SSEND( msg, 1, MPI_INTEGER, 0, 0,
     :                     MPI_COMM_WORLD, ierr )
         end if
         if (myid.eq.0) then
         call MPI_RECV( msg, 1, MPI_INTEGER, ip, 0,
     :                 MPI_COMM_WORLD,MPI_STATUS_IGNORE,ierr )
         end if
      end do
!   task 0 sends a message to all other tasks
      do ip = 1 , nproc -1
         if (myid.eq.0) then
             call MPI_SSEND( msg, 1, MPI_INTEGER, ip, 0,
     :                     MPI_COMM_WORLD, ierr )
         end if
         if (myid.eq.ip) then
         call MPI_RECV( msg, 1, MPI_INTEGER, 0, 0,
     :                 MPI_COMM_WORLD,MPI_STATUS_IGNORE,ierr )
         end if
      end do
      t2 = MPI_WTIME() -t1
 110  format(i2,'  >> start time, duration :',2e8.1)
      call MPI_REDUCE( t2, tbmax, 1, MPI_DOUBLE_PRECISION,
     :                 MPI_MAX, 0, MPI_COMM_WORLD, ierr )
      if (myid.eq.0) then
      write(6,111) tbmax , tbmax*2600.0e+6
 111  format("MPI_BARRIER takes ",e10.2,"sec , i.e.",
     :        f12.0," cycles of 1./2.6e9 sec")
      end if
 200  call MPI_FINALIZE( ierr )

      stop
      end



 
