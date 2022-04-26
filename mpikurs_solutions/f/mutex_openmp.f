      program mutex
      implicit none
c  FORTRAN MPI-INCLUDE-file
      include 'mpif.h'
      include 'omp_lib.h'
      integer    ierr, nproc, myid, tag, stat(MPI_STATUS_SIZE), tp
      integer mxval, cnt, mone, iw, is, idu, iter, tid

c  initialize MPI, get nproc (# of procs) and myid (local proc. id)
      call MPI_INIT(ierr)
!      call MPI_INIT_THREAD(MPI_THREAD_MULTIPLE,tp, ierr )
      call MPI_QUERY_THREAD(tp,ierr)
      write(6,*)MPI_THREAD_SINGLE,MPI_THREAD_FUNNELED
     :        ,MPI_THREAD_SERIALIZED,MPI_THREAD_MULTIPLE, tp
      call MPI_COMM_SIZE( MPI_COMM_WORLD, nproc, ierr )
      call MPI_COMM_RANK( MPI_COMM_WORLD, myid, ierr )
      cnt = 10; mxval = cnt; tag = 0; idu = 0
      if (myid.eq.0) then
        call OMP_SET_NUM_THREADS(2)
!$OMP PARALLEL DEFAULT(SHARED) PRIVATE(tid,iw)
        tid = OMP_GET_THREAD_NUM()
        if (tid.eq.0) then
         if (nproc.gt.1) then
          do iter = 1 , cnt
             call MPI_RECV(idu, 1, MPI_INTEGER, MPI_ANY_SOURCE
     :                   , tag, MPI_COMM_WORLD, stat, ierr)
             is = stat(MPI_SOURCE)
!$OMP CRITICAL
             mxval = mxval-1
             iw = mxval + 1
!$OMP END CRITICAL
             call MPI_SEND(iw, 1, MPI_INTEGER, is, tag,
     :                MPI_COMM_WORLD, ierr)
             if (mxval.le.0) exit
          end do
          do is = 1 , nproc-1
             call MPI_SEND(mxval, 1, MPI_INTEGER, is, tag,
     :                MPI_COMM_WORLD, ierr)
          end do
         end if
        else
           do iter = 1 , cnt
!$OMP CRITICAL
              mxval = mxval - 1
              iw = mxval+1
!$OMP END CRITICAL
              if (iw.le.0) exit  
              call sleep(1+iw/3)
              write(6,*)'on',myid,' >> iw :',iw   
           end do
        end if
!$OMP END PARALLEL
      else 
      do iter = 1 , cnt+1
         call MPI_SEND(idu, 1, MPI_INTEGER, 0, tag,
     :            MPI_COMM_WORLD, ierr)
         call  MPI_RECV( iw, 1, MPI_INTEGER, 0, tag,
     :                 MPI_COMM_WORLD,  stat, ierr )
         if (iw.le.0) exit
         call sleep(1+iw/3)
         write(6,*)'on',myid,' >> iw :',iw
      end do
      end if
      call MPI_FINALIZE( ierr )

      stop
      end



 
