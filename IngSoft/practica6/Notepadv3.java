package es.unizar.eina.notepadv3;

import androidx.appcompat.app.AppCompatActivity;

import android.app.Activity;
import android.content.Intent;
import android.database.Cursor;
import android.os.Bundle;
import android.view.ContextMenu;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ListView;
import android.widget.SimpleCursorAdapter;

import es.unizar.eina.send.SendAbstractionImpl;

public class Notepadv3 extends AppCompatActivity {

    private static final int ACTIVITY_CREATE=0;
    private static final int ACTIVITY_EDIT=1;

    private static final int INSERT_ID = Menu.FIRST;
    private static final int DELETE_ID = Menu.FIRST + 1;
    private static final int EDIT_ID = Menu.FIRST + 2;
    private static final int SEND_ID = Menu.FIRST + 3;
    private static final int TEST_ID = Menu.FIRST + 4;
    private static final int UNDO_TEST_ID = Menu.FIRST + 5;
    private static final int SOBRECARGA_TEST_ID = Menu.FIRST + 6;

    private NotesDbAdapter mDbHelper;
    private ListView mList;

    private static int LAST_VIEWED_NOTE = 0;


    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_notepadv3);

        mDbHelper = new NotesDbAdapter(this);
        mDbHelper.open();
        mList = (ListView)findViewById(R.id.list);
        fillData();

        registerForContextMenu(mList);

    }

    private void fillData() {

        // Get all of the notes from the database and create the item list
        Cursor notesCursor = mDbHelper.fetchAllNotes();

        // Create an array to specify the fields we want to display in the list (only TITLE)
        String[] from = new String[] { NotesDbAdapter.KEY_TITLE };

        // and an array of the fields we want to bind those fields to (in this case just text1)
        int[] to = new int[] { R.id.text1 };

        // Now create an array adapter and set it to display using our row
        SimpleCursorAdapter notes =
                new SimpleCursorAdapter(this, R.layout.notes_row, notesCursor, from, to);
        mList.setAdapter(notes);
        mList.setSelection(LAST_VIEWED_NOTE);
    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        boolean result = super.onCreateOptionsMenu(menu);
        menu.add(Menu.NONE, INSERT_ID, Menu.NONE, R.string.menu_insert);
        menu.add(Menu.NONE, TEST_ID, Menu.NONE, "Test Volumen");
        menu.add(Menu.NONE, UNDO_TEST_ID, Menu.NONE, "Deshacer Test Volumen");
        menu.add(Menu.NONE, SOBRECARGA_TEST_ID, Menu.NONE, "Test Sobrecarga");
        return result;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
            case INSERT_ID:
                createNote();
                //LAST_VIEWED_NOTE = mList.getCount(); //util si no se ordenan las notas alfabeticamente
                return true;
            case TEST_ID:
                runTest();
                return true;
            case UNDO_TEST_ID:
                runUndo();
                return true;
            case SOBRECARGA_TEST_ID:
                runSobrecarga();
                return true;
        }
        return super.onOptionsItemSelected(item);
    }

    @Override
    public void onCreateContextMenu(ContextMenu menu, View v,
                                    ContextMenu.ContextMenuInfo menuInfo) {
        super.onCreateContextMenu(menu, v, menuInfo);
        menu.add(Menu.NONE, DELETE_ID, Menu.NONE, R.string.menu_delete);
        menu.add(Menu.NONE, EDIT_ID, Menu.NONE, R.string.menu_edit);
        menu.add(Menu.NONE, SEND_ID, Menu.NONE, R.string.menu_send);
    }

    @Override
    public boolean onContextItemSelected(MenuItem item) {
        switch(item.getItemId()) {
            case DELETE_ID:
                AdapterView.AdapterContextMenuInfo info = (AdapterView.AdapterContextMenuInfo) item.getMenuInfo();
                LAST_VIEWED_NOTE = info.position;
                mDbHelper.deleteNote(info.id);
                fillData();
                return true;
            case EDIT_ID:
                info = (AdapterView.AdapterContextMenuInfo) item.getMenuInfo();
                LAST_VIEWED_NOTE = info.position;
                editNote(info.position, info.id);
                return true;
            case SEND_ID:
                info = (AdapterView.AdapterContextMenuInfo) item.getMenuInfo();
                LAST_VIEWED_NOTE = info.position;
                //Activity act = (Activity) this.getApplicationContext();
                SendAbstractionImpl aux = new SendAbstractionImpl(this, "mail");
                aux.send("nombre","cuerpo");
        }

        return super.onContextItemSelected(item);
    }

    private void createNote() {
        Intent i = new Intent(this, NoteEdit.class);
        startActivityForResult(i, ACTIVITY_CREATE);
    }


    protected void editNote(int position, long id) {
        Intent i = new Intent(this, NoteEdit.class);
        i.putExtra(NotesDbAdapter.KEY_ROWID, id);
        startActivityForResult(i, ACTIVITY_EDIT);
    }


    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent intent) {
        super.onActivityResult(requestCode, resultCode, intent);
        fillData();
    }



    public void runTest(){
        Test test = new Test(mDbHelper);
        test.pruebaVolumen();
        fillData();
    }
    public void runUndo(){
        Test test = new Test(mDbHelper);
        test.deshacerPruebaVolumen();
        fillData();
    }

    public void runSobrecarga(){
        Test test = new Test(mDbHelper);
        test.pruebaSobreCarga();
        fillData();
    }
}
